#include "postgres.h"
#include "fmgr.h"
#include <stdlib.h>
#include <themis/themis.h>

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(pg_themis_scell_encrypt_seal);
PG_FUNCTION_INFO_V1(pg_themis_scell_decrypt_seal);
PG_FUNCTION_INFO_V1(pg_themis_smessage_encrypt);
PG_FUNCTION_INFO_V1(pg_themis_smessage_decrypt);

Datum pg_themis_scell_encrypt_seal(PG_FUNCTION_ARGS){

  bytea* data = PG_GETARG_BYTEA_P(0);
  bytea* key = PG_GETARG_BYTEA_P(1);
  size_t encrypted_data_len=0;
  bytea* encrypted_data=NULL;

  if(THEMIS_BUFFER_TOO_SMALL!=themis_secure_cell_encrypt_seal((const uint8_t*)VARDATA(key), VARSIZE(key)-VARHDRSZ, NULL, 0, (const uint8_t*)VARDATA(data), VARSIZE(data)-VARHDRSZ, NULL, &encrypted_data_len)){
    ereport(ERROR,(errcode(-1), errmsg("ERR secure seal encription failed"), errdetail("length determination"), NULL));
  }
  encrypted_data = palloc(encrypted_data_len+VARHDRSZ);
  if(!encrypted_data){
    ereport(ERROR,(errcode(-1), errmsg("ERR allocation error"), NULL, NULL));
  }
  SET_VARSIZE(encrypted_data, encrypted_data_len+VARHDRSZ);
  if(THEMIS_SUCCESS!=themis_secure_cell_encrypt_seal((const uint8_t*)VARDATA(key), VARSIZE(key)-VARHDRSZ, NULL, 0, (const uint8_t*)VARDATA(data), VARSIZE(data)-VARHDRSZ, (uint8_t*)VARDATA(encrypted_data), &encrypted_data_len)){
    pfree(encrypted_data);
    ereport(ERROR,(errcode(-1), errmsg("ERR secure seal encription failed"), NULL, NULL));
  }
  PG_RETURN_BYTEA_P(encrypted_data);
}

Datum pg_themis_scell_decrypt_seal(PG_FUNCTION_ARGS){

  bytea* data = PG_GETARG_BYTEA_P(0);
  bytea* key = PG_GETARG_BYTEA_P(1);
  size_t decrypted_data_len=0;
  bytea* decrypted_data=NULL;

  if(THEMIS_BUFFER_TOO_SMALL!=themis_secure_cell_decrypt_seal((const uint8_t*)VARDATA(key), VARSIZE(key)-VARHDRSZ, NULL, 0, (const uint8_t*)VARDATA(data), VARSIZE(data)-VARHDRSZ, NULL, &decrypted_data_len)){
    ereport(ERROR,(errcode(-1), errmsg("ERR secure seal decription failed"), errdetail("length determination"), NULL));
  }
  decrypted_data = palloc(decrypted_data_len+VARHDRSZ);
  if(!decrypted_data){
    ereport(ERROR,(errcode(-1), errmsg("ERR allocation error"), NULL, NULL));
  }
  SET_VARSIZE(decrypted_data, decrypted_data_len+VARHDRSZ);
  if(THEMIS_SUCCESS!=themis_secure_cell_decrypt_seal((const uint8_t*)VARDATA(key), VARSIZE(key)-VARHDRSZ, NULL, 0, (const uint8_t*)VARDATA(data), VARSIZE(data)-VARHDRSZ, (uint8_t*)VARDATA(decrypted_data), &decrypted_data_len)){
    pfree(decrypted_data);
    ereport(ERROR,(errcode(-1), errmsg("ERR secure seal decription failed"), NULL, NULL));
  }
  PG_RETURN_BYTEA_P(decrypted_data);
}

//encrypt data with acra ctruct 
static int smessage_encrypt(const uint8_t* data, const uint32_t data_length, const uint8_t* private_key, const uint32_t private_key_length, const uint8_t* public_key, const uint32_t public_key_length, const uint8_t* peer_public_key, const uint32_t peer_public_key_length, bytea** enc_data){
  size_t encrypted_data_len=0;
  if(THEMIS_BUFFER_TOO_SMALL!=themis_secure_message_wrap(private_key, private_key_length, peer_public_key, peer_public_key_length, data, data_length, NULL, &encrypted_data_len)){
    return -1;
  }
  *enc_data = palloc(public_key_length+sizeof(uint32_t)+encrypted_data_len+VARHDRSZ);
  if(!(*enc_data)){
    return -2;
  }
  memcpy((uint8_t*)VARDATA(*enc_data), &public_key_length, sizeof(public_key_length));
  memcpy((uint8_t*)VARDATA(*enc_data)+sizeof(public_key_length), public_key, public_key_length);
  if(THEMIS_SUCCESS!=themis_secure_message_wrap(private_key, private_key_length, peer_public_key, peer_public_key_length, data, data_length, (uint8_t*)VARDATA(*enc_data)+sizeof(public_key_length)+public_key_length, &encrypted_data_len)){
    pfree(*enc_data);
    return -1;
  }
  encrypted_data_len+=sizeof(public_key_length)+public_key_length;
  SET_VARSIZE(*enc_data, encrypted_data_len+VARHDRSZ);
  return 0;
}

//decrypt  acra structed data 
static int smessage_decrypt(const uint8_t* data, const uint32_t data_length, const uint8_t* private_key, const uint32_t private_key_length, bytea** dec_data){
  size_t decrypted_data_len=0;
  const uint32_t public_key_length = *((const uint32_t*)data);
  const uint8_t* public_key = data+sizeof(uint32_t);
  const uint8_t* data_ = public_key+public_key_length;
  size_t data_length_ = data_length-public_key_length-sizeof(uint32_t); 
  if(data_length<=public_key_length+sizeof(uint32_t)){
    return -1;
  }
  
  if(THEMIS_BUFFER_TOO_SMALL!=themis_secure_message_unwrap(private_key, private_key_length, public_key, public_key_length, data_, data_length_, NULL, &decrypted_data_len)){
    return -1;
  }
  *dec_data = palloc(decrypted_data_len+VARHDRSZ);
  if(!(*dec_data)){
    return -2;
  }
  if(THEMIS_SUCCESS!=themis_secure_message_unwrap(private_key, private_key_length, public_key, public_key_length, data_, data_length_, (uint8_t*)VARDATA(*dec_data), &decrypted_data_len)){
    pfree(*dec_data);
    return -1;
  }
  SET_VARSIZE(*dec_data, decrypted_data_len+VARHDRSZ);
  return 0;
}

static int smessage_enc(const uint8_t* public_key, const uint32_t public_key_length, const uint8_t* data, const uint8_t data_length, bytea** enc_data){
  uint8_t new_private_key[10240];
  uint8_t new_public_key[10240];

  size_t new_private_key_length=10240, new_public_key_length=10240;
  if(THEMIS_SUCCESS!=themis_gen_ec_key_pair(new_private_key, &new_private_key_length, new_public_key, &new_public_key_length)){
    return -1;
  }
  return smessage_encrypt(data, data_length, new_private_key, new_private_key_length, new_public_key, new_public_key_length, public_key, public_key_length, enc_data);
}

static int smessage_dec(const uint8_t* private_key, const uint32_t private_key_length, const uint8_t* data, const uint8_t data_length, bytea** dec_data){
  return smessage_decrypt(data, data_length, private_key, private_key_length, dec_data);
}

Datum pg_themis_smessage_encrypt(PG_FUNCTION_ARGS){
  bytea* data = PG_GETARG_BYTEA_P(0);
  bytea* key = PG_GETARG_BYTEA_P(1);
  bytea* encrypted_data=NULL;

  if(0!=smessage_enc((const uint8_t*)VARDATA(key), VARSIZE(key)-VARHDRSZ, (const uint8_t*)VARDATA(data), VARSIZE(data)-VARHDRSZ, &encrypted_data)){
    ereport(ERROR,(errcode(-1), errmsg("ERR secure message encription failed"), NULL, NULL));
  }
  PG_RETURN_BYTEA_P(encrypted_data);
}
Datum pg_themis_smessage_decrypt(PG_FUNCTION_ARGS){
  bytea* data = PG_GETARG_BYTEA_P(0);
  bytea* key = PG_GETARG_BYTEA_P(1);
  bytea* decrypted_data=NULL;

  if(0!=smessage_dec((const uint8_t*)VARDATA(key), VARSIZE(key)-VARHDRSZ, (const uint8_t*)VARDATA(data), VARSIZE(data)-VARHDRSZ, &decrypted_data)){
    ereport(ERROR,(errcode(-1), errmsg("ERR secure message decription failed"), NULL, NULL));
  }
  PG_RETURN_BYTEA_P(decrypted_data);
}

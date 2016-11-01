#include "postgres.h"
#include "fmgr.h"
#include <stdlib.h>
#include <themis/themis.h>

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(themis_scell_encrypt_seal);
PG_FUNCTION_INFO_V1(themis_scell_decrypt_seal);

Datum themis_scell_encrypt_seal(PG_FUNCTION_ARGS){

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

Datum themis_scell_decrypt_seal(PG_FUNCTION_ARGS){

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

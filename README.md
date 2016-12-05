# PG_Themis
Postgres Themis plugin

=======

pg_themis is a PostgreSQL extension for encryption using the themis library.
Requires PostgreSQL version 9.1 or later.

## Building and installing PG_Themis

Requirements:
to build and install PG_Themis [themis](https://github.com/cossacklabs/themis) must be installed to sysytem

To build and install PG_Themis:

```
make
make install
```

Once PG_Themis is installed, you can add it to a database:

```
CREATE EXTENSION pg_themis;
```

## PG_Themis functions

The extension adds four functions `pg_themis_scell_encrypt_seal`, `pg_themis_scell_decrypt_seal`, `pg_themis_smessage_encrypt`, `pg_themis_smessage_decrypt`, which expose encryption decryption of [Secure Cell](https://github.com/cossacklabs/themis/wiki/Secure-Cell-cryptosystem) in Seal mode and [Secure Cell](https://github.com/cossacklabs/themis/wiki/Secure-Cell-cryptosystem) wrapped in [Secure Message](https://github.com/cossacklabs/themis/wiki/Secure-Message-cryptosystem) with random public key: 

```SQL
-- Encrypts data with key.
CREATE FUNCTION pg_themis_scell_encrypt_seal(data bytea, key bytea)
    RETURNS bytea
    AS 'pg_themis', 'pg_themis_scell_encrypt_seal' LANGUAGE c STRICT;

-- Decrypts data with key.
CREATE FUNCTION pg_themis_scell_decrypt_seal(encrypted_data bytea, key bytea)
    RETURNS bytea
    AS 'pg_themis', 'pg_themis_scell_decrypt_seal' LANGUAGE c STRICT;


-- Encrypts data with public key.
CREATE FUNCTION pg_themis_smessage_encrypt(plain_data bytea, public_key bytea)
    RETURNS bytea
    AS 'pg_themis', 'pg_themis_smessage_encrypt' LANGUAGE c STRICT;

-- Decrypts data with private_key.
CREATE FUNCTION pg_themis_smessage_decrypt(encrypted_data bytea, private_key bytea)
    RETURNS bytea
    AS 'pg_themis', 'pg_themis_smessage_decrypt' LANGUAGE c STRICT;
```

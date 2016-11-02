# pg_themis
Postgres Themis plugin

=======

pg_themis is a PostgreSQL extension for encryption using the themis library.
Requires PostgreSQL version 9.1 or later.

The extension adds two functions:

```SQL
-- Encrypts data with key.
CREATE FUNCTION pg_themis_scell_encrypt_seal(data bytea, key bytea)
    RETURNS bytea
    AS 'pg_themis', 'pg_themis_scell_encrypt_seal' LANGUAGE c STRICT;

-- Decrypts data with key.
CREATE FUNCTION pg_themis_scell_decrypt_seal(data bytea, key bytea)
    RETURNS bytea
    AS 'pg_themis', 'pg_themis_scell_decrypt_seal' LANGUAGE c STRICT;


-- Encrypts data with public key.
CREATE FUNCTION pg_themis_smessage_encrypt(plaintext bytea, key bytea)
    RETURNS bytea
    AS 'pg_themis', 'pg_themis_smessage_encrypt' LANGUAGE c STRICT;

-- Decrypts data with private_key.
CREATE FUNCTION pg_themis_smessage_decrypt(plaintext bytea, key bytea)
    RETURNS bytea
    AS 'pg_themis', 'pg_themis_smessage_decrypt' LANGUAGE c STRICT;
```

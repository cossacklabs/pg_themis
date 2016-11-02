-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION pg_themis" to load this file. \quit

CREATE FUNCTION pg_themis_scell_encrypt_seal(plaintext bytea, key bytea)
	RETURNS bytea
	AS 'pg_themis', 'pg_themis_scell_encrypt_seal' LANGUAGE c STRICT;

CREATE FUNCTION pg_themis_scell_decrypt_seal(plaintext bytea, key bytea)
	RETURNS bytea
	AS 'pg_themis', 'pg_themis_scell_decrypt_seal' LANGUAGE c STRICT;

CREATE FUNCTION pg_themis_smessage_encrypt(plaintext bytea, key bytea)
	RETURNS bytea
	AS 'pg_themis', 'pg_themis_smessage_encrypt' LANGUAGE c STRICT;

CREATE FUNCTION pg_themis_smessage_decrypt(plaintext bytea, key bytea)
	RETURNS bytea
	AS 'pg_themis', 'pg_themis_smessage_decrypt' LANGUAGE c STRICT;

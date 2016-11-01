CREATE EXTENSION themis;

select themis_scell_decrypt_seal(themis_scell_encrypt_seal(E'test data', E'test_password'), E'test_password');

DROP EXTENSION themis;

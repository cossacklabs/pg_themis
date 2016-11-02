CREATE EXTENSION themis;

select pg_themis_scell_decrypt_seal(pg_themis_scell_encrypt_seal(E'test data', E'test_password'), E'test_password');

select pg_themis_smessage_decrypt(pg_themis_smessage_encrypt(E'test data', '\x554543320000002d6bbb797903fab7333a4d6eb7c259de7896fa69e6638691c265a092f65a223ca98ec9a73542'), '\x524543320000002dc7a8ca7a00c3b5d1ad5137308f45e65e54df2b7a45bc8508e8cc3bc9481b631ae8128b3974');

DROP EXTENSION themis;

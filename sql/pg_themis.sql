#
# Copyright (c) 2016 Cossack Labs Limited
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

CREATE EXTENSION pg_themis;

select pg_themis_scell_decrypt_seal(pg_themis_scell_encrypt_seal(E'test data', E'test_password'), E'test_password');

select pg_themis_smessage_decrypt(pg_themis_smessage_encrypt(E'test data', '\x554543320000002d6bbb797903fab7333a4d6eb7c259de7896fa69e6638691c265a092f65a223ca98ec9a73542'), '\x524543320000002dc7a8ca7a00c3b5d1ad5137308f45e65e54df2b7a45bc8508e8cc3bc9481b631ae8128b3974');

DROP EXTENSION pg_themis;

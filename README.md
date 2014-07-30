#dmSecure
Our attempt at making your file-sharing secure.
* AES Encryption for your files.
* Integrity assured using sha2 based mac.
* Share the public-key with friends.
* Share files with friends and foes.
* Filenames on the servers are human-readable hashes of original filenames.

###Building the tool.
The utility contains mainly 3 files. authorize, recover, preprocess.
While an executable is included, the following flags must be set to build each of the files, if a build is desired:
* fpermissive
* lss
* lcrypto

####authorize \<private_key\> \<f1\>
>This command takes takes private_key and filename f1 to generate shareable_key(fk) and a placeholder to hide fn1, fn2.

####preprocess \<shareable_key\> \<f1\>
>This command takes takes shareable_key and file f1 to generate encrypted file f2 and a placeholder to hide fn1, fn2.

####recover \<shareable_key\> \<f2\>
>This command takes takes shareable_key and f2 to generate f3. Available hash of f1 in the payload is compared with derived hash of f3 to ensure integrity.

More about the shematics can be found in the attached documents.

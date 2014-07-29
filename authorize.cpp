/*int aes_encrypt_file(const char * infile, const char * outfile, const void * key, const void * iv, const EVP_CIPHER * cipher, int enc)
{
    assert(cipher != NULL);

    int rc = -1;
    int cipher_block_size = EVP_CIPHER_block_size(cipher);

    assert(cipher_block_size <= BUF_SIZE);

    // The output buffer size needs to be bigger to accomodate incomplete blocks
    // See EVP_EncryptUpdate documentation for explanation:
    //      http://lmgtfy.com/?q=EVP_EncryptUpdate
    int insize = BUF_SIZE;
    int outsize = insize + (cipher_block_size - 1);

    unsigned char inbuf[insize], outbuf[outsize];
    int ofh = -1, ifh = -1;
    int u_len = 0, f_len = 0;

    EVP_CIPHER_CTX ctx;
    EVP_CIPHER_CTX_init(&ctx);

    // Open the input and output files
    rc = AES_ERR_FILE_OPEN;
    if((ifh = open(infile, O_RDONLY)) == -1) {
        fprintf(stderr, "ERROR: Could not open input file %s, errno = %s\n", infile, strerror(errno));
        goto cleanup;
    }

    if((ofh = open(outfile, O_CREAT | O_TRUNC | O_WRONLY, 0644)) == -1) {
        fprintf(stderr, "ERROR: Could not open output file %s, errno = %s\n", outfile, strerror(errno));
        goto cleanup;
    }

    // Initialize the AES cipher for enc/dec
    rc = AES_ERR_CIPHER_INIT;
    if(EVP_CipherInit_ex(&ctx, cipher, NULL, key, iv, enc) == 0) {
        fprintf(stderr, "ERROR: EVP_CipherInit_ex failed. OpenSSL error: %s\n", ERR_error_string(ERR_get_error(), NULL));
        goto cleanup;
    }

    // Read, pass through the cipher, write.
    int read_size, len;
    while((read_size = read(ifh, inbuf, BUF_SIZE)) > 0)
    {
        dbg("Read %d bytes, passing through CipherUpdate...\n", read_size);
        if(EVP_CipherUpdate(&ctx, outbuf, &len, inbuf, read_size) == 0) {
            rc = AES_ERR_CIPHER_UPDATE;
            fprintf(stderr, "ERROR: EVP_CipherUpdate failed. OpenSSL error: %s\n", ERR_error_string(ERR_get_error(), NULL));
            goto cleanup;
        }
        dbg("\tGot back %d bytes from CipherUpdate...\n", len);

        dbg("Writing %d bytes to %s...\n", len, outfile);
        if(write(ofh, outbuf, len) != len) {
            rc = AES_ERR_IO;
            fprintf(stderr, "ERROR: Writing to the file %s failed. errno = %s\n", outfile, strerror(errno));
            goto cleanup;
        }
        dbg("\tWrote %d bytes\n", len);

        u_len += len;
    }

    // Check last read succeeded
    if(read_size == -1) {
        rc = AES_ERR_IO;
        fprintf(stderr, "ERROR: Reading from the file %s failed. errno = %s\n", infile, strerror(errno));
        goto cleanup;
    }

    // Finalize encryption/decryption
    rc = AES_ERR_CIPHER_FINAL;
    if(EVP_CipherFinal_ex(&ctx, outbuf, &f_len) == 0) {
        fprintf(stderr, "ERROR: EVP_CipherFinal_ex failed. OpenSSL error: %s\n", ERR_error_string(ERR_get_error(), NULL));
        goto cleanup;
    }

    dbg("u_len = %d, f_len = %d\n", u_len, f_len);

    // Write the final block, if any
    if(f_len) {
        dbg("Writing final %d bytes to %s...\n", f_len, outfile);
        if(write(ofh, outbuf, f_len) != f_len) {
            rc = AES_ERR_IO;
            fprintf(stderr, "ERROR: Final write to the file %s failed. errno = %s\n", outfile, strerror(errno));
            goto cleanup;
        }
        dbg("\tWrote last %d bytes\n", f_len);
    }

    rc = u_len + f_len;

 cleanup:
    EVP_CIPHER_CTX_cleanup(&ctx);
    if(ifh != -1) close(ifh);
    if(ofh != -1) close(ofh);

    return rc;
}
*/

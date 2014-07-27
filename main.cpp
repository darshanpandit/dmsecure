#include <openssl/hmac.h>
#include<iostream>
#include <stdio.h>
#include <cstring>
#include <openssl/evp.h>

using namespace std;

int main(int argc, char *argv[])
{
    EVP_MD_CTX *mdctx;
    const EVP_MD *md;
    char mess1[] = "Test Message\n";
    char mess2[] = "Hello World\n";
    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len, i;
    OpenSSL_add_all_digests();
    char a ='1';
    const char* digest_algo_code;
    digest_algo_code = &a;
    cout<<digest_algo_code;
    md = EVP_get_digestbyname(digest_algo_code);
    if(!md)
    {
        printf("Unknown message digest %s\n", digest_algo_code);
        exit(1);
    }
    mdctx = EVP_MD_CTX_create();
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, mess1, strlen(mess1));
    EVP_DigestUpdate(mdctx, mess2, strlen(mess2));
    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
    EVP_MD_CTX_destroy(mdctx);
    printf("Digest is: ");
    for(i = 0; i < md_len; i++) printf("%02x", md_value[i]);
    printf("\n");
}

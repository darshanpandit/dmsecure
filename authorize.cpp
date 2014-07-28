#include <openssl/hmac.h>
#include<iostream>
#include<fstream>
#include <stdio.h>
#include <cstring>
#include <openssl/evp.h>

using namespace std;

int write_key(unsigned char *myfile_path, unsigned char* key)
{


    ofstream myfile (myfile_path, ios::out | ios::binary);
    if (myfile.is_open())
    {
        key ='SINGAPORE';
        myfile.write(key, strlen(key));
        myfile.close();
        return 1;
    }
    else cout << "Unable to open file";
    return 0;
}

unsigned char* load_key(char *key_name)
{
    streampos size;
    char * memblock;

    ifstream file (key_name, ios::in|ios::binary|ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        memblock = new char [size];
        file.seekg (0, ios::beg);
        file.read (memblock, size);
        file.close();

        cout << "the entire file content is in memory";
        return memblock;
        delete[] memblock;
    }
    else cout << "Unable to open file";
    return 0;
}



unsigned char* generate_fk(unsigned char *key, unsigned char *filename)
{

    EVP_MD_CTX *mdctx;
    mdctx = EVP_MD_CTX_create();

    EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);

    EVP_DigestUpdate(mdctx, key, strlen(key));
    EVP_DigestUpdate(mdctx, filename, strlen(filename));
    EVP_DigestUpdate(mdctx, key, strlen(key));


    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len;

    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
    EVP_MD_CTX_destroy(mdctx);

    //To Print the hash on screen
    /*
    int i;
    printf("Digest is: ");
    for(i = 0; i < md_len; i++) printf("%02x", md_value[i]);
    printf("\n");
    */
    return md_value;
}


int main(int argc, char *argv[])
{
    char key[]      = "Darshan Pandit";
    char filename[] = "abcderfg.txt";
    unsigned char* final_key;
    unsigned char* final_file;

    final_key   = generate_fk(key,filename);
    final_file  = "mykey";
    int i;
    i           = write_key(final_file,final_key);
    cout<<i;
    /* EVP_MD_CTX *mdctx;
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
     */
}

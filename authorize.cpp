#include <iostream>
#include<fstream>

#include<vector>
#include <iterator>

#include<cstring>
#include <string>
#include<sstream>
#include <iomanip>

#include <openssl/evp.h>
#include <openssl/conf.h>
#include <openssl/err.h>

using namespace std;

int write_data(vector<char> data, char* filename)
{
    std::vector<char>::const_iterator i;
    ofstream myfile (filename, ios::out | ios::binary);
    //vector<char> data;
    //std::copy(data1.begin(), data1.end(), std::back_inserter(data));
    std::copy(data.begin(), data.end(), std::ostream_iterator<char>(myfile, ""));

    return ((data.end()-data.begin())*8);
}

vector<char> read_data(char* filename)
{
    // open the file:
    std::ifstream file(filename, std::ios::binary);
    // read the data:
    vector<char> read_key =  std::vector<char>((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());
    //std::copy(read_key.begin(), read_key.end(), std::ostream_iterator<char>(cout, ""));
    return read_key;
}

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    cout<<"Some Error"<<endl;
    abort();
}

vector<char> compute_hash(char* text,int text_len)
{
    EVP_MD_CTX *mdctx;
    mdctx = EVP_MD_CTX_create();
    EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);
    EVP_DigestUpdate(mdctx, text, text_len);
    char md_value[32];
    int md_len;
    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
    EVP_MD_CTX_destroy(mdctx);
    vector<char> message_digest;
    message_digest.assign(md_value,md_value+32 );
//    std::copy(message_digest.begin(), message_digest.end(), std::ostream_iterator<char>(cout, ""));
    return message_digest;
}

int encrypt1( char* plaintext, int plaintext_len,  char* key,char* iv,  char* ciphertext)
{
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;
    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
    ciphertext_len += len;
    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);
    return ciphertext_len;
}

/**Takes the key , filename, and generates a derived key which is sha256 of E(k,fn)
Returns fk aka Derived key.
*/
vector<char> generate_fk(char *key, char *filename)
{
    char* iv="01234567890123456";
    char ciphertext[32];
    int cipher_len = encrypt1( filename, strlen(filename),  key,iv,  ciphertext);
    vector<char> hash_value = compute_hash(ciphertext,cipher_len);
    return hash_value;

}

std::string get_readable_hash(vector<char> myhash)
{
    char fin[60];
    int i,j;
    j=0;
    for(i=0; i<myhash.size(); i++)
    {
        stringstream ss;
        ss<<std::setw(8)<< std::setfill ('x')<<std::hex <<(int) myhash[i];
        string temp = ss.str();

        fin[j]=temp[6];
        j++;
        fin[j]=temp[7];
        j++;
        // cout<<myhash[i] << " : " <<temp[6]<<temp[7]<<endl;

    }
    return string(fin,64);
}

/**
Input: keyfile, file_to_encrypt
Output: Generates a .key file by the name fn2
Pseudo:
    fk <- hash(E(k,fn1))
    fn2 <- hash(f1)
*/

int main(int argc, char* argv[])
{

    if (argc < 2)   // Check the value of argc. If not enough parameters have been passed, inform user and exit.
    {
        std::cout << "Usage is authorize <keyfile> <file_to_encrypt>"; // Inform the user of how to use the program
        std::cin.get();
        exit(0);
    }
    char* keyfile_path = argv[0];
    char* fn1_path = argv[1];

    //char* keyfile_path = "abcd";
    //char* fn1_path = "README.md";
    vector<char> key = read_data(argv[0]);
    vector<char> fk = generate_fk(&key[0],fn1_path);
    vector<char> fn2_vector = compute_hash(fn1_path, strlen(fn1_path));
    string fn2_name = get_readable_hash(fn2_vector);
    fn2_name.append(".share");

    int success = write_data(fk,&fn2_name[0]);
    cout<<success<<" bits were written in shared_key"<<endl;
    cout<<"Please check for a key to share file as :"<<fn2_name<<endl<<"The file should be stored on the server by the same name. It is fn2."<<endl;
    cout<<"Thank you."<<endl;
    getchar();
    //Write fk to a file by name fn2.share.key.

}

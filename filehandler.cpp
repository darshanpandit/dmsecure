#include<iostream>
#include<fstream>
#include <stdio.h>

#include<vector>
#include <iterator>

#include <openssl/evp.h>
#include <openssl/conf.h>
#include <openssl/err.h>

#include <cstring>



using namespace std;

int write_data(vector<char> data, char* filename){
   std::vector<char>::const_iterator i;
   ofstream myfile (filename, ios::out | ios::binary);
    //vector<char> data;
    //std::copy(data1.begin(), data1.end(), std::back_inserter(data));
   std::copy(data.begin(), data.end(), std::ostream_iterator<char>(myfile, ""));

   return ((data.end()-data.begin())*8);
}

vector<char> read_data(char* filename){
    // open the file:
    cout<<"Opening "<<filename<<endl;
    std::ifstream file(filename, std::ios::binary);
    // read the data:
    vector<char> read_key =  std::vector<char>((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());
    std::copy(read_key.begin(), read_key.end(), std::ostream_iterator<char>(cout, ""));
    return read_key;
}

vector<char> generate_fk1(char *key, char *filename)
{

    EVP_MD_CTX *mdctx;
    mdctx = EVP_MD_CTX_create();

    EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);

    EVP_DigestUpdate(mdctx, key, strlen(key));
    EVP_DigestUpdate(mdctx, filename, strlen(filename));
    EVP_DigestUpdate(mdctx, key, strlen(key));


    char md_value[32];
    int md_len;

    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
    EVP_MD_CTX_destroy(mdctx);

    vector<char> message_digest;
    message_digest.assign(md_value,md_value+32 );
    cout<<"Key Generated : ";
    std::copy(message_digest.begin(), message_digest.end(), std::ostream_iterator<char>(cout, ""));
    cout<<endl;
    cout<<"Message_Digest Length : "<<message_digest.size()<<endl;
    return message_digest;
}

void handle_Errors(void)
{
  ERR_print_errors_fp(stderr);
  abort();
}

int aes_encrypt(char* input_file, char* output_file, char* key_file){
    vector<char> key_vector        =   read_data(key_file);
    vector<char> plaintext_vector  =   read_data(input_file);

    //char* plaintext =   &plaintext_vector[0];
    char plaintext[] = "Darshan is awesome and he is cool as well";
    //char* key       =   &key_vector[0];
    char key[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    char iv[] = {1,2,3,4,5,6,7,8};

    char* ciphertext;
    int len=0;
    int ciphertext_len=0;
    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();

    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, strlen(plaintext)))     handle_Errors();
    ciphertext_len = len;
    //cout<<ciphertext_len<<endl;
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))                            handle_Errors();
    //ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    cout<< ciphertext_len<<endl;
    cout<<"*****************************************"<<endl;
    cout<<"*****************************************"<<endl;
    cout<<"*****************************************"<<endl;
    cout<<"*****************************************"<<endl;
    cout<<"*****************************************"<<endl;

    vector<char> ciphertext_vector(ciphertext, ciphertext_len);
    //int i= 0;//write_data(ciphertext_vector,output_file);
    return 0;
}



int main(){
/*    vector<char> key = generate_fk1("","manas");
//    char arr_key[] = "123456789";
//    key.assign(arr_key,arr_key+strlen(arr_key) );
    char* filename;
    filename = "xyz";
    int bits_written ;
    bits_written = write_data(key,filename);
    cout << bits_written << " bits were written.\n";

    vector<char> read_key = read_data(filename);
    std::copy(read_key.begin(), read_key.end(), std::ostream_iterator<char>(cout, ""));
*/
///*
    char* input_file = "README.md";
    char* output_file= "cipher_readme";
    char* key_file   = "xyz";
    //read_data(input_file);
    //read_data(key_file);
    cout<< aes_encrypt(input_file, output_file, key_file);
//*/
}

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
    cout<<"Decryption Failed"<<endl;
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

int decrypt(char *ciphertext, int ciphertext_len,  char *key,
            char *iv,  char *plaintext)
{
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;
    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
    plaintext_len += len;
    EVP_CIPHER_CTX_free(ctx);
    return plaintext_len;
}


/**
Input: shared_keyfile, file_to_decrypt
Output: Generates a .data file by the name fn3. Integrity is assured.
Pseudo:
    f3. mac_code <- D( fk, f2)
    if mac.code == hash(f3)
        dump f3.
    else
        throw error.
*/

int main(int argc, char* argv[])
{

    if (argc < 2)   // Check the value of argc. If not enough parameters have been passed, inform user and exit.
    {
        std::cout << "Usage is recover <shared_keyfile> <file_to_decrypt>"; // Inform the user of how to use the program
        std::cin.get();
        exit(0);
    }
    char* keyfile_path = argv[0];
    char* fn2_path = argv[1];

    //char* keyfile_path = "b33563x551682c19a781afebcf4dx7bf978fb1f8acx4c6bf87428ed5106870f5.share";
    //char* fn2_path = "b33563x551682c19a781afebcf4dx7bf978fb1f8acx4c6bf87428ed5106870f5";

    vector<char> key_vec = read_data(keyfile_path);
    vector<char> cipher_vec = read_data(fn2_path);
     char* iv="01234567890123456";

    char payload[cipher_vec.size()+16];
    int payload_len= decrypt(&cipher_vec[0],cipher_vec.size(),&key_vec[0],iv,payload);

    vector<char> f1(payload,(payload+(payload_len-32)));
    vector<char> mac_code((payload+(payload_len-32)), (payload+payload_len));

    vector<char> myhash = compute_hash(&f1[0], f1.size());
    string passed_hash = get_readable_hash(mac_code);
    string calculated_hash = get_readable_hash(myhash);

    if(passed_hash.compare(calculated_hash)!=0){
        cout<< "The file was altered,in the transmission. Hence discarding the file.";
        getchar();
        exit(0);
    }
    cout<<"MAC codes matched. Integrity has not been satisfied. Now dumping the uncompressed text."<<endl;
    string fn3 = string(fn2_path,strlen(fn2_path));
    fn3.append(".data");
    write_data(f1,&fn3[0]);
    cout<<"File writtern. Please check : "<<fn3<<" as the file. You may now use this file.";
    getchar();




/*
    //To make a name for the file
    string fn2_final = string(keyfile_path,(strlen(keyfile_path)-6));
    //fn2_final.append(".data");

    int success = write_data(cipher_vec,&fn2_final[0]);
    cout<<success<<" bits were written in the encrypted file"<<endl;
    cout<<"You can now deploy the following file and share it :"<<fn2_final<<endl<<"The file is can be made public. Only those who have fk can read it."<<endl;
    cout<<"Thank you."<<endl;
    getchar();
    //Write fk to a file by name fn2.share.key.
*/
}

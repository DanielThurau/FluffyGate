#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <unistd.h>
#include <ctype.h>

#include "crack/crack.h"
#include "obfuscate.h"
#include "crypto.h"
#include "fileio.h"
#include "packets.h"

#include <string.h>
#define SIZE_PASSWORD 6

struct Devious{
    char *iv;
    int iv_len;
    char *pass;
    int pass_len;
    char **key_set;
    int num_keys;
    char *key;
    u_char *cipher;
    int cipher_len;
};

void dump_devious(struct Devious *d);
int analyze_keys(char **keys, int num_keys, char* iv);


int main(int argc, char *argv[]){
    char *path_to_iv = NULL;
    char *path_to_pass = NULL;
    char *path_to_zip = NULL;
    char *path_to_keys = NULL;
    char *path_to_cipher = NULL;
    int size = 32;
    int c;


    struct Devious breakout;


    opterr = 0;

    while((c = getopt (argc, argv, "i:s:p:z:K:k:c:")) != -1)
        switch(c){
            case 'i':
                path_to_iv = optarg;
                break;
                // key sizes
            case 's':
                size = atoi(optarg);
                break;
            case 'p':
                path_to_pass = optarg;
                break;
            case 'z':
                path_to_zip = optarg;
                break;
            case 'K':
                path_to_keys = optarg;
                break;
            case 'k':
                breakout.key = optarg;
                break;
            case 'c':
                path_to_cipher = optarg;
                break;
            case '?':
                if(optopt == 'i')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                return 1;
            default:
                abort();
        }

   
    if(path_to_iv != NULL){
        u_char *raw = calloc(2*size, sizeof(char));
        int len = unpack(path_to_iv, &raw);
        if(len > 32){
            sanitize(&breakout.iv, raw, size);
            breakout.iv_len = size;
            dump_buffer(breakout.iv, breakout.iv_len);  
        }else if(len == -1){
            breakout.iv = (char*)calloc(size, sizeof(char));
            //printf("%s\n",path_to_iv);
            strncpy(breakout.iv, path_to_iv, size);
            breakout.iv_len = size;    
        }
        
    }

    if(path_to_pass != NULL){
        breakout.pass = password_packet(path_to_pass);
        breakout.pass_len = SIZE_PASSWORD;
        if(breakout.pass == NULL){
            breakout.pass = path_to_pass;
            breakout.pass_len = SIZE_PASSWORD;
        }
        dump_buffer(breakout.pass, strlen(breakout.pass));
    }

    if(path_to_zip != NULL){
        u_char *raw = calloc(2000, sizeof(char));
        int len = unpack(path_to_zip, &raw);
        char *outfile = remove_ext(path_to_zip, '.', '/');
        write_buffer(outfile,raw,len);
        free(outfile);
    }

    if(path_to_keys != NULL){
        char *raw;
        int len;

        // Read all the keys
        len = ReadFile(path_to_keys, &raw);

        // Sanatize buffer of newlines
        if(raw[len-1] == '\n'){
            char* newraw;
            sanitize(&newraw, raw, len-1);
            free(raw);
            raw = newraw; len = len -1;
        }

        breakout.key_set = (char**) calloc(len - size, sizeof(char*));
        breakout.num_keys = deobfuscate(raw, size, breakout.key_set);
    }



    if(path_to_cipher != NULL){
        u_char *raw;
        char* deciphertext = calloc(1000, sizeof(u_char));
        breakout.cipher_len = unpack(path_to_cipher, &raw);
        breakout.cipher = (u_char*)calloc(breakout.cipher_len, sizeof(u_char));
        // char **cipher = calloc(1,sizeof(char*));
        // printf("%s\n",raw);
        breakout.cipher = raw;
        // printf("%s\n",*cipher);

        // write_buffer("")
        /* 
            test that all of my data is good so far
        */ 
        int correct = analyze_keys(breakout.key_set, breakout.num_keys, breakout.iv);
        int incorrect = breakout.num_keys - correct;


        printf("Number of correct keys   : %d\nNumber of incorrect keys : %d\n",correct, incorrect);





        for(int i = 0; i < breakout.num_keys; i++){
            int deciphertext_len = decrypt(breakout.cipher, breakout.cipher_len, breakout.key_set[i], breakout.iv, deciphertext);
            // int deciphertext_len = decrypt(breakout.cipher, breakout.cipher_len, "BEDDEE76C5B9DD1CC753F7DFB2986D37", "CA3879E06C455CE1EF427B7E6C3B635D", deciphertext);
            if(deciphertext_len != -1){
                printf("this key worked: %s\n",breakout.key_set[i]);
                printf("%s",deciphertext);
                break;
            }
        }

   }
}


int analyze_keys(char **keys, int num_keys, char* iv){
    int successful_keys = 0;
    for(int i = 0; i < num_keys; i++){
        if(test_iv_key(keys[i], iv) == 0){
            successful_keys++;
        }
    }
    return successful_keys;
}

void dump_devious(struct Devious *d){
    printf("IV length: %d\nIV: ",d->iv_len);
    dump_buffer(d->iv, d->iv_len);
    printf("Password length: %d\nPassword: ",d->pass_len);
    dump_buffer(d->pass, d->pass_len);
}

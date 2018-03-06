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
    }

    if(path_to_zip != NULL){
        u_char *raw = calloc(2000, sizeof(char));
        int len = unpack(path_to_zip, &raw);

        write_buffer("test.zip",raw,len);
    }

    if(path_to_keys != NULL){
        char *raw;
        int len;

        len = ReadFile(path_to_keys, &raw);

        breakout.key_set = (char**) calloc(len - size, sizeof(char*));
        breakout.num_keys = deobfuscate(raw, size, breakout.key_set);

    }

    if(path_to_cipher != NULL){
        u_char *raw; //= calloc(2000, sizeof(u_char));
        char* deciphertext = calloc(1000, sizeof(u_char));
        breakout.cipher_len = unpack(path_to_cipher, &raw);
        breakout.cipher = (u_char*)calloc(breakout.cipher_len, sizeof(u_char));
        breakout.cipher = raw;
        // strncpy(breakout.cipher, raw, breakout.cipher_len);

        dump_buffer(breakout.cipher, breakout.cipher_len);
        printf("--------------------------------------\n");



        // char *cipher_text = calloc(2000, sizeof(char));
        // // int len = ReadFile("backups/message.cipher", &cipher_text);
        // printf("noped: %d\n", len);
        // dump_buffer(cipher_text, len);
        // printf("\n--------------------------------------\n");


/*

        printf("%d\n", breakout.num_keys);
        for(int i = 0; i < breakout.num_keys; i++){
            int deciphertext_len = decrypt(breakout.cipher, breakout.cipher_len, "0C8513C2D0A7A07F3BE8744090BF10E3", breakout.iv, deciphertext);
            // int deciphertext_len = decrypt(breakout.cipher, breakout.cipher_len, "BEDDEE76C5B9DD1CC753F7DFB2986D37", "CA3879E06C455CE1EF427B7E6C3B635D", deciphertext);
            if(deciphertext_len != -1){
                printf("this key worked: %s\n",breakout.key_set[i]);
                printf("%s\n",deciphertext);
                break;
            }
            printf("\n---------------\n%s\n---------------------\n",breakout.key_set[i]);
            printf("%s\n",deciphertext);
        }
*/
    }

    // printf("%s\n",breakout.iv);

    // dump_devious(&breakout);
}





void dump_devious(struct Devious *d){
    printf("IV length: %d\nIV: ",d->iv_len);
    dump_buffer(d->iv, d->iv_len);
    printf("Password length: %d\nPassword: ",d->pass_len);
    dump_buffer(d->pass, d->pass_len);
}

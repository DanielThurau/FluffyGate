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
#define WORDCOUNT_TRIGGER 10.0
#define PERCENT_MATCH 0.05


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
int check_heuristic(char *text, int text_len);
int check_ascii(char *text, int text_len);
void rot_k(char *e_text, int e_text_len, char**d_text, int k);


int main(int argc, char *argv[]){
    char *path_to_iv = NULL;
    char *path_to_pass = NULL;
    char *path_to_zip = NULL;
    char *path_to_keys = NULL;
    char *path_to_cipher = NULL;
    char *path_to_unknown = NULL;
    int size = 32;
    int c;


    struct Devious breakout;


    opterr = 0;

    while((c = getopt (argc, argv, "i:s:p:z:K:k:c:u:")) != -1)
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
            case 'u':
                path_to_unknown = optarg;
                break;
            case '?':
                if(optopt == 'i')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                return 1;
            default:
                abort();
        }
    if(path_to_unknown != NULL){
        u_char *raw = calloc(1000, sizeof(char));
        int len = unpack(path_to_unknown, &raw);
        if(len > 0)
            dump_buffer(raw, len);  
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
        breakout.cipher = raw;

        // write_buffer("")
        /* 
            test that all of my data is good so far
        */ 
        int correct = analyze_keys(breakout.key_set, breakout.num_keys, breakout.iv);
        int incorrect = breakout.num_keys - correct;


        printf("Number of correct keys   : %d\nNumber of incorrect keys : %d\n",correct, incorrect);





        for(int i = 0; i < breakout.num_keys; i++){
            int deciphertext_len = decrypt(breakout.cipher, breakout.cipher_len, breakout.key_set[i], breakout.iv, deciphertext);
            if(deciphertext_len != -1){
                if(check_ascii(deciphertext, deciphertext_len)){
                    // dump_buffer(deciphertext, deciphertext_len);
                    char **d_text = (char**)calloc(1,sizeof(char*));
                    for(int j = 0; j < 27; j++){

                        rot_k(deciphertext, deciphertext_len, d_text, j);
                        // dump_buffer(d_text[0], deciphertext_len);
                        
                    }
                    // if(strstr(deciphertext, "dthurau") != NULL){
                    //     write_buffer("test.txt",deciphertext, deciphertext_len);
                    // }

                }
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

int check_ascii(char *text, int text_len){
    float ascii_char = 0;
    for(int i = 0; i < text_len; i++){
        if(isalpha(text[i]) || isdigit(text[i])){
            ascii_char++;
        }
    }
    if(ascii_char/(float)text_len > 0.50){
        return 1;
    }else{
        return 0;
    }


}


int check_heuristic(char *text, int text_len){
    const char * heuristic[][1] = {
        {"of"},
        {"the"},
        {"that"},
        {"thou"},
        {"ye"},
        {"thee"},
        {"thine"},
        {"thy"},
        {"art"},
        {"dost"},
        {"doth"},
        {"ere"},
        {"hast"},
        {"tis"},
        {"and"},
        {"i"},
        {"I"},
        {"to"},
        {"you"},
        {"my"},
        {"in"},
        {"is"},
        {"not"},
        {"with"},
        {"me"},
        {"it"}
    };

    // count words
    float total_words = 0;
    for(int i = 0; i < text_len; i++){
        if(text[i] == ' '){
            total_words++;
        }
    }

    // count word matches of english dictionary
    float matches = 0;
    for(int i = 0; i < sizeof(heuristic)/sizeof(heuristic[0]); i++){
        // count duplicates?
        if(strstr(text, heuristic[i][0]) != NULL){
            matches++;
        }
    }

    // if there are enough words, and a certain percent is matched
    float d = matches / total_words;
    if(d > PERCENT_MATCH && total_words > WORDCOUNT_TRIGGER){
        return 1;
    }else{
        return 0;
    }

}

void rot_k(char *e_text, int e_text_len, char**d_text, int k){
    char ch;
    *d_text = (char*)calloc(e_text_len, sizeof(char));
    
    for(int i = 0; i < e_text_len; ++i){
        ch = e_text[i];
        if(ch >= 'a' && ch <= 'z'){
            ch = ch - k;
            if(ch < 'a'){
                ch = ch + 'z' - 'a' + 1;
            }
            
            d_text[0][i] = ch;
        }
        else if(ch >= 'A' && ch <= 'Z'){
            ch = ch - k;
            
            if(ch < 'A'){
                ch = ch + 'Z' - 'A' + 1;
            }
            
            d_text[0][i] = ch;
        }
    }
    dump_buffer(d_text[0], e_text_len);
    printf("\n");
    
}
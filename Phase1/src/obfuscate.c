#include "obfuscate.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int deobfuscate(char *data, int key_length, char **keys){

    int num_keys = strlen(data) - key_length; 

    for(int i = 0; i < num_keys; i++){
        keys[i] = calloc(32,sizeof(char));
        strncpy(keys[i], &data[i], key_length);
    }
    return num_keys;
}
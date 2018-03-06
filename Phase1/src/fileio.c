#include <stdio.h>
#include <stdlib.h>


#include "fileio.h"
void dump_buffer(void *buffer, int buffer_size){
    int i;
    for(i = 0;i < buffer_size; ++i)
        printf("%c",((char *)buffer)[i]);
    // printf("\n");
}


int ReadFile(char *name, char** buffer){
    FILE *file;
    unsigned long fileLen;

    //Open file
    file = fopen(name, "rb");
    if (!file){
        fprintf(stderr, "Unable to open file %s", name);
        return -1;
    }
    
    //Get file length
    fseek(file, 0, SEEK_END);
    fileLen=ftell(file);
    fseek(file, 0, SEEK_SET);

    //Allocate memory
    *buffer=(char *)malloc(fileLen+1);
    if (!buffer){
        fprintf(stderr, "Memory error!");
                                fclose(file);
        return -1;
    }

    //Read file contents into buffer
    fread(*buffer, fileLen, 1, file);
    fclose(file);

    return fileLen;

}

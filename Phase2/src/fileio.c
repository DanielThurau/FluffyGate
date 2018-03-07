#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileio.h"

void dump_buffer(void *buffer, int buffer_size){
    for(int i = 0;i < buffer_size; ++i)
        printf("%c",((char *)buffer)[i]);
    // printf("\n");
}

int write_buffer(char *fname, void *buffer, int buffer_size){
    FILE *fp = fopen(fname, "wb");
    if (!fp){
        fprintf(stderr, "Unable to open file %s", fname);
        return -1;
    }

    for(int i = 0;i < buffer_size; ++i)
        putc(((char *)buffer)[i], fp);
    fclose(fp);

}

int set_buffer(char **destination, void *source, int buffer_size){
    *destination=(char *)malloc(buffer_size);
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

char *remove_ext (char* mystr, char dot, char sep){
    char *retstr, *lastdot, *lastsep;

    // Error checks and allocate string.

    if (mystr == NULL)
        return NULL;
    if ((retstr = malloc (strlen (mystr) + 1)) == NULL)
        return NULL;

    // Make a copy and find the relevant characters.

    strcpy (retstr, mystr);
    lastdot = strrchr (retstr, dot);
    lastsep = (sep == 0) ? NULL : strrchr (retstr, sep);

    // If it has an extension separator.

    if (lastdot != NULL) {
        // and it's before the extenstion separator.

        if (lastsep != NULL) {
            if (lastsep < lastdot) {
                // then remove it.

                *lastdot = '\0';
            }
        } else {
            // Has extension separator with no path separator.

            *lastdot = '\0';
        }
    }

    // Return the modified string.

    return retstr;
}
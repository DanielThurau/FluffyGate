#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <unistd.h>
#include <ctype.h>

#include "fileio.h"
#include "packets.h"

#include <string.h>


int main(int argc, char *argv[]){
 
   
    u_char *raw = calloc(2*32, sizeof(char));
    int len = unpack(argv[1], &raw);
    if(len > 1){
        return 0;
    }else{
        return 1;
    }  
}
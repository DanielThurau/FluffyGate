#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <unistd.h>
#include <ctype.h>

#include "fileio.h"
#include "packets.h"
#include "sort.h"
#include <string.h>


int filter(char *packet_path){
    u_char *raw = calloc(2*32, sizeof(char));
    int len = unpack(packet_path, &raw);
    if(len > 1){
        return 1;
    }else{
        return 0;
    } 
}

// int is_password(char *raw)
#include <stdio.h> //printf
#include <stdlib.h>
#include <string.h>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <time.h>
#include <math.h>

#include "fileio.h"
#include "listener.h"
#include "sort.h"
#include "packets.h"

int my_listen(char *dest_IP, int port_num, char *save_dir){
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
    
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        perror("Could not create socket");
        return 1;
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr(dest_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(port_num);
 
    printf("Connecting to %s:%d....\n", dest_IP, port_num);

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    printf("Connected to %s:%d\n", dest_IP, port_num);
     
    int counter = 0;
    //keep communicating with server
    while(1)
    {
        //Receive a reply from the server
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            perror("recv failed");
            break;
        }
        
        char name[50];
        sprintf(name, "%s%d.pcap",save_dir, counter);
        printf("%s\n",name);
        // if(filter)
        write_buffer(name, server_reply, 2000);

        // u_char *raw = calloc(1000, sizeof(char));
        // int len = unpack_buffer(server_reply, &raw);
        // if(len > 0)
        //     dump_buffer(raw, len);  

        printf("Received Something\n");
        counter++;
    }
     
    close(sock);
}
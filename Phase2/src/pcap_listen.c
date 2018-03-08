/*
    C ECHO client example using sockets
*/
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

long get_current_time_with_ms ();

int main(int argc , char *argv[]){
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
    
    char *dest_IP = argv[1];
    int port_num = atoi(argv[2]);



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
        // sprintf(name, "data/P2/T3/%ld.pcap", get_current_time_with_ms);
        sprintf(name, "data/P2/T5/%d.pcap", counter);

        write_buffer(name, server_reply, 2000);
        printf("Received Something\n");
        counter++;
    }
     
    close(sock);
    return 0;
}

//https://stackoverflow.com/questions/3756323/how-to-get-the-current-time-in-milliseconds-from-c-in-linux/17083824
long get_current_time_with_ms (){
    long            ms; // Milliseconds
    time_t          s;  // Seconds
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);

    s  = spec.tv_sec*1000;
    ms = round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds
    if (ms > 999) {
        s+=1000;
        ms = 0;
    }

    return (long)(s + ms);
}
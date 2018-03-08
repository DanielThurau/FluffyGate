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
#include "listener.h"


long get_current_time_with_ms ();

int main(int argc , char *argv[]){
    my_listen(argv[1], atoi(argv[2]), "data/P2/T4/");
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
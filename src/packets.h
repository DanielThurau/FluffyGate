#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <unistd.h>
#include <ctype.h>

#include "osi_headers.h"
#include "crack/crack.h"
#include "obfuscate.h"
#include "crypto.h"
#include "fileio.h"

#include <string.h>
#define SIZE_PASSWORD 6
#define SIZE_ETHERNET 14


int get_payload(const struct pcap_pkthdr *header, const u_char *packet, u_char **payload);

int unpack(const char* path_to_pcap_file, u_char **payload);

char *password_packet(const char *path_to_pass_file);

void sanitize(char **destination, char *source, int size);

// int unpack_buffer(char *buffer, u_char **payload);
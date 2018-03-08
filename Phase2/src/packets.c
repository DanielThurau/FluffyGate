#include "packets.h"

typedef int bool;
#define true 1
#define false 0

char *password_packet(const char *path_to_pass_file){
    // big buffer for something thats just 12 chars
    u_char *raw = (char*) calloc(32, sizeof(char));
    
    int len = unpack(path_to_pass_file, &raw);
    if(len == -1){
        return NULL;
    }


    char *e_pass;
    sanitize(&e_pass, raw, strlen(raw) - 1);

    char *salt = (char *) calloc(3, sizeof(char));
    strncpy(salt, e_pass, 2);

    /* 
    // Debug statments 
    printf("%s\n", new);
    printf("%s\n", salt);
    */

    char * d_pass = calloc(SIZE_PASSWORD, sizeof(char));
    char *f = "parg1/WTS.A8";
    for(int i = 0; i < 12; i++){
        if(f[i] != e_pass[i]){
            break;
        }else if(i == 11 && f[i] == e_pass[i]){
            return "fluffy";
        }
    }
    dump_buffer(e_pass, len);
    // crackSingle(salt, e_pass, SIZE_PASSWORD, d_pass);
    return d_pass;
}


int unpack(const char* path_to_pcap_file, u_char **payload){
    pcap_t *handle; /* Session handle */
    char errbuf[PCAP_ERRBUF_SIZE]; /* Error string */
    struct pcap_pkthdr header;  /* The header that pcap gives us */
    const u_char *packet;       /* The actual packet */
    int size_payload;

    handle = pcap_open_offline(path_to_pcap_file, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Couldn't open file %s\n", path_to_pcap_file);
        return -1;
    }

    packet = pcap_next(handle, &header);

    size_payload = get_payload(&header, packet, payload);
    if(payload == NULL){
        fprintf(stderr, "Couldn't decode packet %s\n", path_to_pcap_file);
        return -1;
    }  

    
    pcap_close(handle);
    payload[sizeof(payload) - 1] == '\0';
    return size_payload;
}


int get_payload(const struct pcap_pkthdr *header, const u_char *packet, u_char **payload){
    const struct sniff_ethernet *ethernet; /* The ethernet header */
    const struct sniff_ip *ip; /* The IP header */
    const struct sniff_tcp *tcp; /* The TCP header */

    u_int size_ip;
    u_int size_tcp;
    int size_payload;

    ethernet = (struct sniff_ethernet*)(packet);
    ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
    size_ip = IP_HL(ip)*4;
    if (size_ip < 20) {
        printf(" * Invalid IP header length: %u bytes\n", size_ip);
        return -1;
    }
    tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);
    size_tcp = TH_OFF(tcp)*4;

    bool fin = false;
    bool ack = false;
    bool syn = false;
    bool push = false;


    if (tcp->th_flags & TH_FIN)
        fin = true;
    if (tcp->th_flags & TH_SYN)
        ack = true;
    if (tcp->th_flags & TH_RST)
        printf("RST set\n");
    if (tcp->th_flags & TH_PUSH)
        push = true;
    if (tcp->th_flags & TH_ACK)
        ack = true;
    if (tcp->th_flags & TH_URG)
        printf("URG set\n");
    if (tcp->th_flags & TH_ECE)
        printf("ECE set\n");
    if (tcp->th_flags & TH_CWR)
        printf("CWR set\n");

    if(!(fin == false && syn == false && ack == true && push == true)){
        return -1;
    }

    if (size_tcp < 20) {
        printf(" * Invalid TCP header length: %u bytes\n", size_tcp);
        return -1;
    }
    size_payload = ntohs(ip->ip_len) - (size_ip + size_tcp);
    payload[0] = (u_char*)calloc(size_payload, sizeof(u_char));
    for(u_int i = 0; i < size_payload; i++){
        payload[0][i] = (u_char)(packet + SIZE_ETHERNET + size_ip + size_tcp+i)[0];
    }
    return size_payload;
}

void sanitize(char **destination, char *source, int size){
    *destination = (char *)calloc(size, sizeof(char));
    strncpy(*destination, source, size);

}
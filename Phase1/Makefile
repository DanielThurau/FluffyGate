pcap_open: src/pcap_open.c src/crack/crack.* src/obfuscate*  src/fileio.*
	gcc -o pcap_open src/pcap_open.c src/osi_headers.h src/crack/crack.* src/obfuscate* src/crypto* src/fileio.* -lpcap -lcrypt -lcrypto -lssl

test_crypto: src/test_crypto.c src/crypto.h src/crypto.c
	gcc -o test_crypto src/test_crypto.c src/crypto.h src/crypto.c -lcrypto

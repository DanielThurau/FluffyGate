FILEIO = src/fileio.h src/fileio.c
CRYPTO = src/crypto.h src/crypto.c
OBF = src/obfuscate.h src/obfuscate.c
CRACK = src/crack/crack.h src/crack/crack.c
PACKETS = src/packets.h src/packets.c

HEADERS = src/osi_headers.h
LIBS = -lpcap -lcrypt -lcrypto -lssl

MAIN_LISTEN = src/pcap_listen.c
MAIN_OPEN = src/pcap_open.c
MAIN_TEST =src/test_crypto.c


all: pcap_listen pcap_open

debug:
	chmod +x bin/run.sh 
	bin/run.sh true

test:
	chmod +x bin/run.sh
	bin/run.sh false


pcap_listen: $(MAIN_LISTEN) $(CRACK) $(OBF) $(FILEIO)
	gcc -o bin/pcap_listen $(MAIN_LISTEN) $(HEADERS) $(CRACK) $(OBF) $(CRYPTO) $(FILEIO) $(PACKETS) $(LIBS)


pcap_open: $(MAIN_OPEN) $(CRACK) $(OBF) $(FILEIO)
	gcc -o bin/pcap_open $(MAIN_OPEN) $(HEADERS) $(CRACK) $(OBF) $(CRYPTO) $(FILEIO) $(PACKETS) $(LIBS)

test_crypto: src/test_crypto.c src/crypto.h src/crypto.c
	gcc -o bin/test_crypto src/test_crypto.c src/crypto.h src/crypto.c -lcrypto

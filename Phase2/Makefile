FILEIO = src/fileio.h src/fileio.c
CRYPTO = src/crypto.h src/crypto.c
OBF = src/obfuscate.h src/obfuscate.c
CRACK = src/crack/crack.h src/crack/crack.c
PACKETS = src/packets.h src/packets.c
LISTENER = src/listener.h src/listener.c

HEADERS = src/osi_headers.h
LIBS = -lpcap -lcrypt -lcrypto -lssl -lm

MAIN_LISTEN = src/pcap_listen.c
MAIN_OPEN = src/pcap_open.c
MAIN_TEST = src/test_crypto.c
MAIN_SORT = src/pcap_sort.c

all: pcap_listen pcap_open pcap_sort

test:
	chmod +x ./bin/test_suite.sh
	./bin/test_suite.sh


pcap_listen: $(MAIN_LISTEN) $(FILEIO) $(PACKETS) $(LISTENER) $(CRACK)
	gcc -o bin/pcap_listen $(MAIN_LISTEN) $(CRACK) $(FILEIO) $(PACKETS) $(HEADERS) $(LISTENER) $(LIBS)

pcap_open: $(MAIN_OPEN) $(CRACK) $(OBF) $(FILEIO) $(PACKETS)
	gcc -o bin/pcap_open $(MAIN_OPEN) $(HEADERS) $(CRACK) $(OBF) $(CRYPTO) $(FILEIO) $(PACKETS) $(LIBS)

pcap_sort: $(MAIN_SORT) $(FILEIO) $(PACKETS)
	gcc -o bin/pcap_sort $(MAIN_SORT) $(HEADERS) $(FILEIO) $(PACKETS) $(LIBS)

test_crypto: src/test_crypto.c src/crypto.h src/crypto.c
	gcc -o bin/test_crypto src/test_crypto.c src/crypto.h src/crypto.c -lcrypto

#!/bin/bash

debug=$1

echo "What is your data directory?"

if [ "$debug" = true ]; then
	data_dir="data/P1/pre/"
elif [ "$debug" = false ]; then
	data_dir="data/P1/mine/"
else
	read data_dir
fi



# ===========================================
# Password stuff
# ===========================================

echo "What is the password [file]?"

if [ "$debug" = true ]; then
	pcap_pass="fluffy"
elif [ "$debug" = false ]; then
	pcap_pass="uLMZh6"
else
	read pcap_pass 
fi

pass=$(./bin/pcap_open -p $pcap_pass 2>.log)



# ============================================
# Zip file stuff
# ============================================
echo "What is the zip [file]"

if [ "$debug" = true ]; then
	pcap_zipfile=$data_dir"key.zip.pcap"
elif [ "$debug" = false ]; then
	pcap_zipfile=$data_dir"dthurau.key.zip.pcap"
else
	read pcap_zipfile
fi

./bin/pcap_open -z $pcap_zipfile 

zipfile=$(echo $pcap_zipfile | grep -Po '.*(?=\.)')


python ./python/unzipping.py $pass $zipfile

cp key $data_dir


# =============================================
# IV stuff
# =============================================
echo "What is the iv [file]"

if [ "$debug" = true ]; then
	pcap_ivfile=$data_dir"iv.pcap"
elif [ "$debug" = false ]; then
	pcap_ivfile=$data_dir"dthurau.iv.pcap"
else
	read pcap_ivfile
fi

iv=$(./bin/pcap_open -i $pcap_ivfile 2>.log)

ivfile=$data_dir"iv.plain"


printf $iv > $ivfile

# ============================================
# Cipher stuff
# ============================================

echo "What is the cipher file"

if [ "$debug" = true ]; then
	pcap_cipherfile=$data_dir"message.pcap"
elif [ "$debug" = false ]; then
	pcap_cipherfile=$data_dir"dthurau.message.pcap"
else
	read pcap_cipherfile
fi

./bin/pcap_open -i $iv -c $pcap_cipherfile -K $data_dir"key" 2>.log



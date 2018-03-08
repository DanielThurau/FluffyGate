#!/bin/bash

debug=$1


echo "What is your data directory?"

if [ "$debug" = true ]; then
	data_dir=$2
else
	read data_dir
fi



# ===========================================
# Password stuff
# ===========================================

echo "What is the password [file]?"

if [ "$debug" = true ]; then
	pcap_pass=$3
else
	read pcap_pass 
fi
pass=$(./bin/pcap_open -p $pcap_pass 2>.log)


# ============================================
# Zip file stuff
# ============================================
echo "What is the zip [file]"

if [ "$debug" = true ]; then
	pcap_zipfile=$4
else
	read pcap_zipfile
fi

./bin/pcap_open -z $pcap_zipfile 

zipfile=$(echo $pcap_zipfile | grep -Po '.*(?=\.)')
echo $zipfile 

python ./python/unzipping.py $pass $zipfile

rm $zipfile
mv key $data_dir


# =============================================
# IV stuff
# =============================================
echo "What is the iv [file]"

if [ "$debug" = true ]; then
	pcap_ivfile=$5
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
	pcap_cipherfile=$6
else
	read pcap_cipherfile
fi


echo "What is the decode style?"
if [ "$debug" = true ]; then
	decode_method=$7
else
	read decode_method
fi


echo "./bin/pcap_open -i $iv -c $pcap_cipherfile -K $data_dir"key" -d $decode_method 2>.log"

if [ -z "$decode_method" ]
then
	./bin/pcap_open -i $iv -c $pcap_cipherfile -K $data_dir"key" 2>.log
else
	./bin/pcap_open -i $iv -c $pcap_cipherfile -K $data_dir"key" -d $decode_method 2>.log
fi


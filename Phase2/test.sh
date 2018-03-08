#!/bin/bash
for f in data/P2/T1/*; do
    echo $f >> all_files
    # ./bin/pcap_open -u $f
  done


# while IFS=$' \t\n\r' read -r LINE; do
#     # echo $LINE
#     echo "$LINE" 128.114.59.29 45923 | nc 128.114.59.42 2001
# done < passwords.txt

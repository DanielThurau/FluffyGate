#!/bin/bash
while IFS=$' \t\n\r' read -r LINE; do
    # echo $LINE
    tcpdump -r $LINE -w - | nc 127.0.0.1 45923
    sleep 0.1
done < all_files
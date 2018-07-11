#!/bin/bash
FILES=data/P2/T1/*
myvar=0

for f in $FILES
do
    ./bin/pcap_sort $f 2> /dev/null
    OUT=$?
    if [ $OUT -eq 0 ];then
        myvar=$(($myvar + 1))
        echo $f >> good_files
    fi
done
echo $myvar

python python/process_incoming.py
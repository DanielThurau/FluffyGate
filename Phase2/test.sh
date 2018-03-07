while read p; do
    echo $p
    ./bin/pcap_open -i $p  
    echo
done < true_sort

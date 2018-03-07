while read p; do
    echo $p
    ./bin/pcap_open -p $p  
    echo
done < true_sort

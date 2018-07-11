#!/bin/bash
for f in *; do
      ../pcap_open -i $f
  done

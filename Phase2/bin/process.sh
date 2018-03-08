#!/bin/bash

declare -A passwords=( ["moo"]="cow" ["woof"]="dog")

# Create new file handle 5
exec 5< true_sort

# Now you can use "<&5" to read from this file
while read line1 <&5 ; do
        read line2 <&5
        read line3 <&5
        read line4 <&5




        # ./bin/run.sh "true" "cur/" $line1 $line2 $line3 $line4 $line5
        # echo "Four lines: $line1 $line2 $line3 $line4 $line5"
done

# Close file handle 5
exec 5<&-

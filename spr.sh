#!/bin/bash

# rename files in tests/trees/ beginning with unprtr* to indicate their leaf number and SPR distance

count=0
for filename in tests/trees/unprtr*; do
    ((count++))
    echo $count
    file=$(basename "$filename" )
    spr=$(rspr -fpt <"tests/trees/$file" | tail -n 1 |sed 's/[^0-9]*//g')
    size=$(cat "tests/trees/"$file | grep -o "[(,]*[0-9a-zA-Z]*[,)]" | grep -o "[0-9a-zA-Z]*" | sort -n | uniq | wc -l)
    mv "tests/trees/"$file "tests/trees/tree"$count"_"$size"_"$spr".txt"
done

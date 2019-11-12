#!/bin/bash

# append SPR distance to the end of the files for usage with this program

for filename in tests/trees/*.txt; do
    file=$(basename "$filename" )
    spr=$(echo "tests/trees/$file" | grep -o '[0-9]*.txt' | grep -o '[0-9]*')
    echo $spr >> "tests/trees/"$file
done

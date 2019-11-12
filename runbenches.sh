#!/bin/bash

# run google benchmarks for the files in tests/trees and output to csv

echo 'RUNNING BENCHMARKS...'

for filename in tests/trees/*.txt; do
    file=$(basename "$filename" )
    echo 'running tests/trees/'$file
    ./cmake-build-debug/benchmark/cspr_benchmark --benchmark_format=csv < 'tests/trees/'$file | tail -n 3 > temp.csv
    size=$(echo "tests/tree/$file" | grep -o '_[0-9]*_' | grep -o '[0-9]*')
    spr=$(echo "tests/trees/$file" | grep -o '[0-9]*.txt' | grep -o '[0-9]*')
    p=''
    for i in $(seq 1 3);
    do
	    p=$p', '$size', '$spr'\n'
    done
    echo -e $p
    echo -e $p > tmppaste
    paste temp.csv tmppaste >> bm.csv
    rm tmppaste
    rm temp.csv
done

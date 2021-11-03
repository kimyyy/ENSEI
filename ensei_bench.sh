#!/bin/bash

poly_degrees=(2048 4096 8192 16384)
err=ensei_result/err.log

function ensei_benchmark(){
    dirname=ensei_result/multipoly
    mkdir -p $dirname
    for poly_degree in ${poly_degrees[@]}
    do
        echo "Frequency-Domain Convolution: (poly_degree) = ($poly_degree)"
        output_path="$dirname/ensei$poly_degree.txt"
        # assume executable file is build/bin/ensei_bench
        env build/bin/ensei_bench $poly_degree 2>$err  > $output_path
    done
}

function clean(){
    rm -rf ensei_result/*
}

ensei_benchmark

# ENSEI: Frequency-Domain Homomorphic Convolution
This is an example implementation of frequency-domain homomorphic convolution over
RLWE-based ciphertexts based on the [SEAL library](https://github.com/microsoft/SEAL "SEAL").

## Requirements
- cmake (>= 3.16.3) 
- C++ compiler with C++17 compatibility

## Quick Start
Please also refer to README.md for building the SEAL library. Here, we only
provide a simplified version of the build process on Ubuntu 20.04.2 LTS.

First, build SEAL by running

```sh
cmake -S . -B build
cmake --build build
```

The binary for running the linear transformation will be in build/bin/ensei_bench. 
To run the benchmark, simply execute the binary with proper arguments, e.g.,
```sh
./ensei_bench 2048
```
for running a direct convolution with polynomial degree 2048

 You can also run 
```sh
./ensei_bench.sh
```
to benchmark a set of pre-defined computations, where the benchmarked results
will be in the folder ensei_result/

## CMake Options

| CMake option | Values | Information |
| --- | --- | --- |
| ENSEI_FETCH_THIRDPARTY | **ON**/OFF | Automatically download and build dependecies

## Directory Layout
- Benchmarks
  - src: Benchmarks

## Publication
The academic background and detailed explanation can be found [here](https://arxiv.org/abs/2003.05328). Feel free to cite this work as follows.

```bibtex
@InProceedings{Bian_2020_CVPR,
author = {Bian, Song and Wang, Tianchen and Hiromoto, Masayuki and Shi, Yiyu and Sato, Takashi},
title = {ENSEI: Efficient Secure Inference via Frequency-Domain Homomorphic Convolution for Privacy-Preserving Visual Recognition},
booktitle = {IEEE/CVF Conference on Computer Vision and Pattern Recognition (CVPR)},
month = {June},
year = {2020}
}
```

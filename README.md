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
will be in the folder hlt_result/

## CMake Options

| CMake option | Values | Information |
| --- | --- | --- |
| ENSEI_FETCH_THIRDPARTY | **ON**/OFF | Automatically download and build dependecies

## Directory Layout
- Benchmarks
  - src: Benchmarks

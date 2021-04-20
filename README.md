<img src="https://guinsoodb.org/images/GuinsooDB_Logo_dl.png" height="50">

![.github/workflows/main.yml](https://github.com/cwida/guinsoodb/workflows/.github/workflows/main.yml/badge.svg?branch=master)
[![CodeFactor](https://www.codefactor.io/repository/github/cwida/guinsoodb/badge)](https://www.codefactor.io/repository/github/cwida/guinsoodb)
[![Coverage Status](https://coveralls.io/repos/github/cwida/guinsoodb/badge.svg?branch=master)](https://coveralls.io/github/cwida/guinsoodb?branch=master)
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.3901452.svg)](https://zenodo.org/record/3901452)


## Installation
If you just want to install and use GuinsooDB, please see [our website](https://www.guinsoodb.org) for installation and usage instructions.

## Development 
For development, GuinsooDB requires [CMake](https://cmake.org), Python3 and a `C++11` compliant compiler. Run `make` in the root directory to compile the sources. For development, use `make debug` to build a non-optimized debug version. You should run `make unit` and `make allunit` to verify that your version works properly after making changes. To test performance, you can run several standard benchmarks from the root directory by executing `./build/release/benchmark/benchmark_runner`.

Please also refer to our [Contribution Guide](CONTRIBUTING.md).



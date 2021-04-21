# GuinsooDB

![logo](./logo/guinsoodb.svg)

`GuinsooDB, not only fast.`

<br/>


## Development 

For development, GuinsooDB requires [CMake](https://cmake.org), Python3 and a `C++11` 
compliant compiler. Run `make` in the root directory to compile the sources. 

For development, use `make debug` to build a non-optimized debug version. 
You should run `make unit` and `make allunit` to verify that your version works properly 
after making changes. To test performance, you can run several standard benchmarks from 
the root directory by executing `./build/release/benchmark/benchmark_runner`.

For more information, please start [here](https://ciusji.github.io/guinsoo/).

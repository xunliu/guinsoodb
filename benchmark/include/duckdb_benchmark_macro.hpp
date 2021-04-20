//===----------------------------------------------------------------------===//
//
//                         GuinsooDB
//
// guinsoodb_benchmark_macro.hpp
//
// Author: Mark Raasveldt
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb_benchmark.hpp"

#define DUCKDB_BENCHMARK(NAME, GROUP)                                                                                  \
	class NAME##Benchmark : public GuinsooDBBenchmark {                                                                   \
		NAME##Benchmark(bool register_benchmark) : GuinsooDBBenchmark(register_benchmark, "" #NAME, GROUP) {              \
		}                                                                                                              \
                                                                                                                       \
	public:                                                                                                            \
		static NAME##Benchmark *GetInstance() {                                                                        \
			static NAME##Benchmark singleton(true);                                                                    \
			auto benchmark = unique_ptr<GuinsooDBBenchmark>(new NAME##Benchmark(false));                                  \
			return &singleton;                                                                                         \
		}

#define REGISTER_BENCHMARK(NAME) auto global_instance_##NAME = NAME##Benchmark::GetInstance()

#define FINISH_BENCHMARK(NAME)                                                                                         \
	}                                                                                                                  \
	;                                                                                                                  \
	REGISTER_BENCHMARK(NAME);

namespace guinsoodb {}

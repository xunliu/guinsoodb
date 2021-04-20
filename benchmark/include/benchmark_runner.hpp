//===----------------------------------------------------------------------===//
//
//                         GuinsooDB
//
// benchmark_runner.hpp
//
// Author: Mark Raasveldt
//
//===----------------------------------------------------------------------===//

#pragma once

#include "benchmark_configuration.hpp"
#include "benchmark.hpp"
#include "guinsoodb/common/constants.hpp"
#include "guinsoodb/common/fstream.hpp"

namespace guinsoodb {
class GuinsooDB;

//! The benchmark runner class is responsible for running benchmarks
class BenchmarkRunner {
	BenchmarkRunner();

public:
	static constexpr const char *GUINSOODB_BENCHMARK_DIRECTORY = "guinsoodb_benchmark_data";
	BenchmarkConfiguration configuration;

	static BenchmarkRunner &GetInstance() {
		static BenchmarkRunner instance;
		return instance;
	}

	//! Save the current database state, exporting it to a set of CSVs in the GUINSOODB_BENCHMARK_DIRECTORY directory
	static void SaveDatabase(GuinsooDB &db, string name);
	//! Try to initialize the database from the GUINSOODB_BENCHMARK_DIRECTORY
	static bool TryLoadDatabase(GuinsooDB &db, string name);

	//! Register a benchmark in the Benchmark Runner, this is done automatically
	//! as long as the proper macro's are used
	static void RegisterBenchmark(Benchmark *benchmark);

	void Log(string message);
	void LogLine(string message);
	void LogResult(string message);
	void LogOutput(string message);

	void RunBenchmark(Benchmark *benchmark);
	void RunBenchmarks();

	vector<Benchmark *> benchmarks;
	ofstream out_file;
	ofstream log_file;
};

} // namespace guinsoodb

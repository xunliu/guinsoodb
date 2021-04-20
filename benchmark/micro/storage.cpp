#include "benchmark_runner.hpp"
#include "guinsoodb_benchmark_macro.hpp"
#include "guinsoodb/main/appender.hpp"

using namespace guinsoodb;

GUINSOODB_BENCHMARK(SELECT1Memory, "[storage]")
void Load(GuinsooDBBenchmarkState *state) override {
}
void RunBenchmark(GuinsooDBBenchmarkState *state) override {
	for (int32_t i = 0; i < 50000; i++) {
		state->conn.Query("SELECT 1");
	}
}
string VerifyResult(QueryResult *result) override {
	return string();
}
string BenchmarkInfo() override {
	return "Run the query \"SELECT 1\" 50K times in in-memory mode";
}
FINISH_BENCHMARK(SELECT1Memory)

GUINSOODB_BENCHMARK(SELECT1Disk, "[storage]")
void Load(GuinsooDBBenchmarkState *state) override {
}
void RunBenchmark(GuinsooDBBenchmarkState *state) override {
	for (int32_t i = 0; i < 50000; i++) {
		state->conn.Query("SELECT 1");
	}
}
string VerifyResult(QueryResult *result) override {
	return string();
}
bool InMemory() override {
	return false;
}
string BenchmarkInfo() override {
	return "Run the query \"SELECT 1\" 50K times in in-memory mode";
}
FINISH_BENCHMARK(SELECT1Disk)

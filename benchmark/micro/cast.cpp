#include "benchmark_runner.hpp"
#include "guinsoodb_benchmark_macro.hpp"
#include "guinsoodb/main/appender.hpp"

#include <random>

using namespace guinsoodb;

#define CAST_COUNT 10000000

GUINSOODB_BENCHMARK(CastDateToString, "[cast]")
void Load(GuinsooDBBenchmarkState *state) override {
	std::uniform_int_distribution<> year_dist(1990, 2010), day_dist(1, 28), month_dist(1, 12);
	std::mt19937 gen;
	gen.seed(42);

	state->conn.Query("CREATE TABLE dates(d DATE);");
	Appender appender(state->conn, "dates");
	// insert the elements into the database
	for (int i = 0; i < CAST_COUNT; i++) {
		appender.AppendRow(Value::DATE(year_dist(gen), month_dist(gen), day_dist(gen)));
	}
}
string GetQuery() override {
	return "SELECT CAST(d AS VARCHAR) FROM dates";
}
string VerifyResult(QueryResult *result) override {
	if (!result->success) {
		return result->error;
	}
	return string();
}
string BenchmarkInfo() override {
	return "Cast date to string";
}
FINISH_BENCHMARK(CastDateToString)

GUINSOODB_BENCHMARK(CastTimeToString, "[cast]")
void Load(GuinsooDBBenchmarkState *state) override {
	std::uniform_int_distribution<> hour_dist(0, 23), min_dist(0, 59);
	std::mt19937 gen;
	gen.seed(42);

	state->conn.Query("CREATE TABLE times(d TIME);");
	Appender appender(state->conn, "times");
	// insert the elements into the database
	for (int i = 0; i < CAST_COUNT; i++) {
		appender.AppendRow(Value::TIME(hour_dist(gen), min_dist(gen), min_dist(gen), 0));
	}
}
string GetQuery() override {
	return "SELECT CAST(d AS VARCHAR) FROM times";
}
string VerifyResult(QueryResult *result) override {
	if (!result->success) {
		return result->error;
	}
	return string();
}
string BenchmarkInfo() override {
	return "Cast time to string";
}
FINISH_BENCHMARK(CastTimeToString)

GUINSOODB_BENCHMARK(CastTimestampToString, "[cast]")
void Load(GuinsooDBBenchmarkState *state) override {
	std::uniform_int_distribution<> year_dist(1990, 2010), day_dist(1, 28), month_dist(1, 12);
	std::uniform_int_distribution<> hour_dist(0, 23), min_dist(0, 59);
	std::mt19937 gen;
	gen.seed(42);

	state->conn.Query("CREATE TABLE timestamps(d TIMESTAMP);");
	Appender appender(state->conn, "timestamps");
	// insert the elements into the database
	for (int i = 0; i < CAST_COUNT; i++) {
		appender.AppendRow(Value::TIMESTAMP(year_dist(gen), month_dist(gen), day_dist(gen), hour_dist(gen),
		                                    min_dist(gen), min_dist(gen), 0));
	}
}
string GetQuery() override {
	return "SELECT CAST(d AS VARCHAR) FROM timestamps";
}
string VerifyResult(QueryResult *result) override {
	if (!result->success) {
		return result->error;
	}
	return string();
}
string BenchmarkInfo() override {
	return "Cast timestamp to string";
}
FINISH_BENCHMARK(CastTimestampToString)

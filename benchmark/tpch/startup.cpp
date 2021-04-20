#include "benchmark_runner.hpp"
#include "compare_result.hpp"
#include "tpch-extension.hpp"
#include "guinsoodb_benchmark_macro.hpp"

using namespace guinsoodb;

#define SF 1

#define TPCHStartup(QUERY)                                                                                             \
	string db_path = "guinsoodb_benchmark_db.db";                                                                         \
	void Load(GuinsooDBBenchmarkState *state) override {                                                                  \
		DeleteDatabase(db_path);                                                                                       \
		{                                                                                                              \
			GuinsooDB db(db_path);                                                                                        \
			db.LoadExtension<TPCHExtension>();                                                                         \
			Connection con(db);                                                                                        \
			con.Query("CALL dbgen(sf=" + std::to_string(SF) + ")");                                                    \
		}                                                                                                              \
		{                                                                                                              \
			auto config = GetConfig();                                                                                 \
			config->checkpoint_wal_size = 0;                                                                           \
			GuinsooDB db(db_path, config.get());                                                                          \
		}                                                                                                              \
	}                                                                                                                  \
	void RunBenchmark(GuinsooDBBenchmarkState *state) override {                                                          \
		auto config = GetConfig();                                                                                     \
		GuinsooDB db(db_path, config.get());                                                                              \
		db.LoadExtension<TPCHExtension>();                                                                             \
		Connection con(db);                                                                                            \
		state->result = con.Query(QUERY);                                                                              \
	}                                                                                                                  \
	string BenchmarkInfo() override {                                                                                  \
		return string("Start a TPC-H SF1 database and run ") + QUERY + string(" in the database");                     \
	}

#define NormalConfig()                                                                                                 \
	unique_ptr<DBConfig> GetConfig() {                                                                                 \
		return make_unique<DBConfig>();                                                                                \
	}

GUINSOODB_BENCHMARK(TPCHEmptyStartup, "[startup]")
TPCHStartup("SELECT * FROM lineitem WHERE 1=0") NormalConfig() string VerifyResult(QueryResult *result) override {
	if (!result->success) {
		return result->error;
	}
	return string();
}
FINISH_BENCHMARK(TPCHEmptyStartup)

GUINSOODB_BENCHMARK(TPCHCount, "[startup]")
TPCHStartup("SELECT COUNT(*) FROM lineitem") NormalConfig() string VerifyResult(QueryResult *result) override {
	if (!result->success) {
		return result->error;
	}
	return string();
}
FINISH_BENCHMARK(TPCHCount)

GUINSOODB_BENCHMARK(TPCHSimpleAggr, "[startup]")
TPCHStartup("SELECT SUM(l_extendedprice) FROM lineitem") NormalConfig() string
    VerifyResult(QueryResult *result) override {
	if (!result->success) {
		return result->error;
	}
	return string();
}
FINISH_BENCHMARK(TPCHSimpleAggr)

GUINSOODB_BENCHMARK(TPCHQ1, "[startup]")
TPCHStartup("PRAGMA tpch(1)") NormalConfig() string VerifyResult(QueryResult *result) override {
	if (!result->success) {
		return result->error;
	}
	return compare_csv(*result, TPCHExtension::GetAnswer(SF, 1), true);
}
FINISH_BENCHMARK(TPCHQ1)

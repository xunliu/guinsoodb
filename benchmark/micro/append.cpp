#include "benchmark_runner.hpp"
#include "guinsoodb_benchmark_macro.hpp"
#include "guinsoodb/main/appender.hpp"

using namespace guinsoodb;

//////////////
// INSERT //
//////////////
#define APPEND_BENCHMARK_INSERT(CREATE_STATEMENT, AUTO_COMMIT)                                                         \
	void Load(GuinsooDBBenchmarkState *state) override {                                                                  \
		state->conn.Query(CREATE_STATEMENT);                                                                           \
	}                                                                                                                  \
	void RunBenchmark(GuinsooDBBenchmarkState *state) override {                                                          \
		if (!AUTO_COMMIT)                                                                                              \
			state->conn.Query("BEGIN TRANSACTION");                                                                    \
		for (int32_t i = 0; i < 100000; i++) {                                                                         \
			state->conn.Query("INSERT INTO integers VALUES (" + std::to_string(i) + ")");                              \
		}                                                                                                              \
		if (!AUTO_COMMIT)                                                                                              \
			state->conn.Query("COMMIT");                                                                               \
	}                                                                                                                  \
	void Cleanup(GuinsooDBBenchmarkState *state) override {                                                               \
		state->conn.Query("DROP TABLE integers");                                                                      \
		Load(state);                                                                                                   \
	}                                                                                                                  \
	string VerifyResult(QueryResult *result) override {                                                                \
		return string();                                                                                               \
	}                                                                                                                  \
	string BenchmarkInfo() override {                                                                                  \
		return "Append 100K 4-byte integers to a table using a series of INSERT INTO statements";                      \
	}

GUINSOODB_BENCHMARK(Append100KIntegersINSERT, "[append]")
APPEND_BENCHMARK_INSERT("CREATE TABLE integers(i INTEGER)", false)
FINISH_BENCHMARK(Append100KIntegersINSERT)

GUINSOODB_BENCHMARK(Append100KIntegersINSERTDisk, "[append]")
APPEND_BENCHMARK_INSERT("CREATE TABLE integers(i INTEGER)", false)
bool InMemory() override {
	return false;
}
FINISH_BENCHMARK(Append100KIntegersINSERTDisk)

GUINSOODB_BENCHMARK(Append100KIntegersINSERTPrimary, "[append]")
APPEND_BENCHMARK_INSERT("CREATE TABLE integers(i INTEGER PRIMARY KEY)", false)
FINISH_BENCHMARK(Append100KIntegersINSERTPrimary)

GUINSOODB_BENCHMARK(Append100KIntegersINSERTAutoCommit, "[append]")
APPEND_BENCHMARK_INSERT("CREATE TABLE integers(i INTEGER)", true)
FINISH_BENCHMARK(Append100KIntegersINSERTAutoCommit)

//////////////
// PREPARED //
//////////////
struct GuinsooDBPreparedState : public GuinsooDBBenchmarkState {
	unique_ptr<PreparedStatement> prepared;

	GuinsooDBPreparedState(string path) : GuinsooDBBenchmarkState(path) {
	}
	virtual ~GuinsooDBPreparedState() {
	}
};

#define APPEND_BENCHMARK_PREPARED(CREATE_STATEMENT)                                                                    \
	unique_ptr<GuinsooDBBenchmarkState> CreateBenchmarkState() override {                                                 \
		auto result = make_unique<GuinsooDBPreparedState>(GetDatabasePath());                                             \
		return move(result);                                                                                           \
	}                                                                                                                  \
	void Load(GuinsooDBBenchmarkState *state_p) override {                                                                \
		auto state = (GuinsooDBPreparedState *)state_p;                                                                   \
		state->conn.Query(CREATE_STATEMENT);                                                                           \
		state->prepared = state->conn.Prepare("INSERT INTO integers VALUES ($1)");                                     \
	}                                                                                                                  \
	void RunBenchmark(GuinsooDBBenchmarkState *state_p) override {                                                        \
		auto state = (GuinsooDBPreparedState *)state_p;                                                                   \
		state->conn.Query("BEGIN TRANSACTION");                                                                        \
		for (int32_t i = 0; i < 100000; i++) {                                                                         \
			state->prepared->Execute(i);                                                                               \
		}                                                                                                              \
		state->conn.Query("COMMIT");                                                                                   \
	}                                                                                                                  \
	void Cleanup(GuinsooDBBenchmarkState *state) override {                                                               \
		state->conn.Query("DROP TABLE integers");                                                                      \
		Load(state);                                                                                                   \
	}                                                                                                                  \
	string VerifyResult(QueryResult *result) override {                                                                \
		return string();                                                                                               \
	}                                                                                                                  \
	string BenchmarkInfo() override {                                                                                  \
		return "Append 100K 4-byte integers to a table using a series of prepared INSERT INTO statements";             \
	}

GUINSOODB_BENCHMARK(Append100KIntegersPREPARED, "[append]")
APPEND_BENCHMARK_PREPARED("CREATE TABLE integers(i INTEGER)")
FINISH_BENCHMARK(Append100KIntegersPREPARED)

GUINSOODB_BENCHMARK(Append100KIntegersPREPAREDDisk, "[append]")
APPEND_BENCHMARK_PREPARED("CREATE TABLE integers(i INTEGER)")
bool InMemory() override {
	return false;
}
FINISH_BENCHMARK(Append100KIntegersPREPAREDDisk)

GUINSOODB_BENCHMARK(Append100KIntegersPREPAREDPrimary, "[append]")
APPEND_BENCHMARK_PREPARED("CREATE TABLE integers(i INTEGER PRIMARY KEY)")
FINISH_BENCHMARK(Append100KIntegersPREPAREDPrimary)

//////////////
// APPENDER //
//////////////
#define APPEND_BENCHMARK_APPENDER(CREATE_STATEMENT)                                                                    \
	void Load(GuinsooDBBenchmarkState *state) override {                                                                  \
		state->conn.Query(CREATE_STATEMENT);                                                                           \
	}                                                                                                                  \
	void RunBenchmark(GuinsooDBBenchmarkState *state) override {                                                          \
		state->conn.Query("BEGIN QUERY");                                                                              \
		Appender appender(state->conn, "integers");                                                                    \
		for (int32_t i = 0; i < 100000; i++) {                                                                         \
			appender.BeginRow();                                                                                       \
			appender.Append<int32_t>(i);                                                                               \
			appender.EndRow();                                                                                         \
		}                                                                                                              \
		appender.Close();                                                                                              \
		state->conn.Query("COMMIT");                                                                                   \
	}                                                                                                                  \
	void Cleanup(GuinsooDBBenchmarkState *state) override {                                                               \
		state->conn.Query("DROP TABLE integers");                                                                      \
		Load(state);                                                                                                   \
	}                                                                                                                  \
	string VerifyResult(QueryResult *result) override {                                                                \
		return string();                                                                                               \
	}                                                                                                                  \
	string BenchmarkInfo() override {                                                                                  \
		return "Append 100K 4-byte integers to a table using an Appender";                                             \
	}

GUINSOODB_BENCHMARK(Append100KIntegersAPPENDER, "[append]")
APPEND_BENCHMARK_APPENDER("CREATE TABLE integers(i INTEGER)")
FINISH_BENCHMARK(Append100KIntegersAPPENDER)

GUINSOODB_BENCHMARK(Append100KIntegersAPPENDERDisk, "[append]")
APPEND_BENCHMARK_APPENDER("CREATE TABLE integers(i INTEGER)")
bool InMemory() override {
	return false;
}
FINISH_BENCHMARK(Append100KIntegersAPPENDERDisk)

GUINSOODB_BENCHMARK(Append100KIntegersAPPENDERPrimary, "[append]")
APPEND_BENCHMARK_APPENDER("CREATE TABLE integers(i INTEGER PRIMARY KEY)")
FINISH_BENCHMARK(Append100KIntegersAPPENDERPrimary)

///////////////
// COPY INTO //
///////////////
#define APPEND_BENCHMARK_COPY(CREATE_STATEMENT)                                                                        \
	void Load(GuinsooDBBenchmarkState *state) override {                                                                  \
		state->conn.Query("CREATE TABLE integers(i INTEGER)");                                                         \
		Appender appender(state->conn, "integers");                                                                    \
		for (int32_t i = 0; i < 100000; i++) {                                                                         \
			appender.BeginRow();                                                                                       \
			appender.Append<int32_t>(i);                                                                               \
			appender.EndRow();                                                                                         \
		}                                                                                                              \
		appender.Close();                                                                                              \
		state->conn.Query("COPY integers TO 'integers.csv' DELIMITER '|'");                                            \
		state->conn.Query("DROP TABLE integers");                                                                      \
		state->conn.Query(CREATE_STATEMENT);                                                                           \
	}                                                                                                                  \
	string GetQuery() override {                                                                                       \
		return "COPY integers FROM 'integers.csv' DELIMITER '|'";                                                      \
	}                                                                                                                  \
	void Cleanup(GuinsooDBBenchmarkState *state) override {                                                               \
		state->conn.Query("DROP TABLE integers");                                                                      \
		state->conn.Query(CREATE_STATEMENT);                                                                           \
	}                                                                                                                  \
	string VerifyResult(QueryResult *result) override {                                                                \
		return string();                                                                                               \
	}                                                                                                                  \
	string BenchmarkInfo() override {                                                                                  \
		return "Append 100K 4-byte integers to a table using the COPY INTO statement";                                 \
	}

GUINSOODB_BENCHMARK(Append100KIntegersCOPY, "[append]")
APPEND_BENCHMARK_COPY("CREATE TABLE integers(i INTEGER)")
FINISH_BENCHMARK(Append100KIntegersCOPY)

GUINSOODB_BENCHMARK(Append100KIntegersCOPYDisk, "[append]")
APPEND_BENCHMARK_COPY("CREATE TABLE integers(i INTEGER)")
bool InMemory() override {
	return false;
}
FINISH_BENCHMARK(Append100KIntegersCOPYDisk)

GUINSOODB_BENCHMARK(Append100KIntegersCOPYPrimary, "[append]")
APPEND_BENCHMARK_COPY("CREATE TABLE integers(i INTEGER PRIMARY KEY)")
FINISH_BENCHMARK(Append100KIntegersCOPYPrimary)

GUINSOODB_BENCHMARK(Write100KIntegers, "[append]")
void Load(GuinsooDBBenchmarkState *state) override {
	state->conn.Query("CREATE TABLE integers(i INTEGER)");
	Appender appender(state->conn, "integers");
	for (int32_t i = 0; i < 100000; i++) {
		appender.BeginRow();
		appender.Append<int32_t>(i);
		appender.EndRow();
	}
}
string GetQuery() override {
	return "COPY integers TO 'integers.csv' DELIMITER '|' HEADER";
}
string VerifyResult(QueryResult *result) override {
	if (!result->success) {
		return result->error;
	}
	return string();
}
string BenchmarkInfo() override {
	return "Write 100K 4-byte integers to CSV";
}
FINISH_BENCHMARK(Write100KIntegers)

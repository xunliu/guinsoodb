#include "catch.hpp"
#include "guinsoodb.h"
#include "test_helpers.hpp"
#include "guinsoodb/common/exception.hpp"

using namespace guinsoodb;
using namespace std;

class CAPIResult {
public:
	~CAPIResult() {
		guinsoodb_destroy_result(&result);
	}
	void Query(guinsoodb_connection connection, string query) {
		success = (guinsoodb_query(connection, query.c_str(), &result) == GuinsooDBSuccess);
	}

	idx_t ColumnCount() {
		return result.column_count;
	}

	idx_t row_count() {
		return result.row_count;
	}

	template <class T>
	T Fetch(idx_t col, idx_t row) {
		throw NotImplementedException("Unimplemented type for fetch");
	}

	bool IsNull(idx_t col, idx_t row) {
		return result.columns[col].nullmask[row];
	}

	string ColumnName(idx_t col) {
		auto colname = guinsoodb_column_name(&result, col);
		return colname ? string(colname) : string();
	}

public:
	bool success = false;

private:
	guinsoodb_result result;
};

static bool NO_FAIL(CAPIResult &result) {
	return result.success;
}

static bool NO_FAIL(unique_ptr<CAPIResult> result) {
	return NO_FAIL(*result);
}

template <>
bool CAPIResult::Fetch(idx_t col, idx_t row) {
	return guinsoodb_value_boolean(&result, col, row);
}

template <>
int8_t CAPIResult::Fetch(idx_t col, idx_t row) {
	return guinsoodb_value_int8(&result, col, row);
}

template <>
int16_t CAPIResult::Fetch(idx_t col, idx_t row) {
	return guinsoodb_value_int16(&result, col, row);
}

template <>
int32_t CAPIResult::Fetch(idx_t col, idx_t row) {
	return guinsoodb_value_int32(&result, col, row);
}

template <>
int64_t CAPIResult::Fetch(idx_t col, idx_t row) {
	return guinsoodb_value_int64(&result, col, row);
}

template <>
float CAPIResult::Fetch(idx_t col, idx_t row) {
	return guinsoodb_value_float(&result, col, row);
}

template <>
double CAPIResult::Fetch(idx_t col, idx_t row) {
	return guinsoodb_value_double(&result, col, row);
}

template <>
guinsoodb_date CAPIResult::Fetch(idx_t col, idx_t row) {
	auto data = (guinsoodb_date *)result.columns[col].data;
	return data[row];
}

template <>
guinsoodb_timestamp CAPIResult::Fetch(idx_t col, idx_t row) {
	auto data = (guinsoodb_timestamp *)result.columns[col].data;
	return data[row];
}

template <>
guinsoodb_blob CAPIResult::Fetch(idx_t col, idx_t row) {
	auto data = (guinsoodb_blob *)result.columns[col].data;
	return data[row];
}

template <>
string CAPIResult::Fetch(idx_t col, idx_t row) {
	auto value = guinsoodb_value_varchar(&result, col, row);
	string strval = string(value);
	free((void *)value);
	return strval;
}

class CAPITester {
public:
	CAPITester() : database(nullptr), connection(nullptr) {
	}
	~CAPITester() {
		Cleanup();
	}

	void Cleanup() {
		if (connection) {
			guinsoodb_disconnect(&connection);
			connection = nullptr;
		}
		if (database) {
			guinsoodb_close(&database);
			database = nullptr;
		}
	}

	bool OpenDatabase(const char *path) {
		Cleanup();
		if (guinsoodb_open(path, &database) != GuinsooDBSuccess) {
			return false;
		}
		if (guinsoodb_connect(database, &connection) != GuinsooDBSuccess) {
			return false;
		}
		return true;
	}

	unique_ptr<CAPIResult> Query(string query) {
		auto result = make_unique<CAPIResult>();
		result->Query(connection, query);
		return result;
	}

	guinsoodb_database database = nullptr;
	guinsoodb_connection connection = nullptr;
};

TEST_CASE("Basic test of C API", "[capi]") {
	CAPITester tester;
	unique_ptr<CAPIResult> result;

	// open the database in in-memory mode
	REQUIRE(tester.OpenDatabase(nullptr));

	// select scalar value
	result = tester.Query("SELECT CAST(42 AS BIGINT)");
	REQUIRE_NO_FAIL(*result);
	REQUIRE(result->ColumnCount() == 1);
	REQUIRE(result->row_count() == 1);
	REQUIRE(result->Fetch<int64_t>(0, 0) == 42);
	REQUIRE(!result->IsNull(0, 0));

	// select scalar NULL
	result = tester.Query("SELECT NULL");
	REQUIRE_NO_FAIL(*result);
	REQUIRE(result->ColumnCount() == 1);
	REQUIRE(result->row_count() == 1);
	REQUIRE(result->Fetch<int64_t>(0, 0) == 0);
	REQUIRE(result->IsNull(0, 0));

	// select scalar string
	result = tester.Query("SELECT 'hello'");
	REQUIRE_NO_FAIL(*result);
	REQUIRE(result->ColumnCount() == 1);
	REQUIRE(result->row_count() == 1);
	REQUIRE(result->Fetch<string>(0, 0) == "hello");
	REQUIRE(!result->IsNull(0, 0));

	result = tester.Query("SELECT 1=1");
	REQUIRE_NO_FAIL(*result);
	REQUIRE(result->ColumnCount() == 1);
	REQUIRE(result->row_count() == 1);
	REQUIRE(result->Fetch<bool>(0, 0) == true);
	REQUIRE(!result->IsNull(0, 0));

	result = tester.Query("SELECT 1=0");
	REQUIRE_NO_FAIL(*result);
	REQUIRE(result->ColumnCount() == 1);
	REQUIRE(result->row_count() == 1);
	REQUIRE(result->Fetch<bool>(0, 0) == false);
	REQUIRE(!result->IsNull(0, 0));

	result = tester.Query("SELECT i FROM (values (true), (false)) tbl(i) group by i order by i");
	REQUIRE_NO_FAIL(*result);
	REQUIRE(result->ColumnCount() == 1);
	REQUIRE(result->row_count() == 2);
	REQUIRE(result->Fetch<bool>(0, 0) == false);
	REQUIRE(result->Fetch<bool>(0, 1) == true);
	REQUIRE(!result->IsNull(0, 0));

	// multiple insertions
	REQUIRE_NO_FAIL(tester.Query("CREATE TABLE test (a INTEGER, b INTEGER);"));
	REQUIRE_NO_FAIL(tester.Query("INSERT INTO test VALUES (11, 22)"));
	REQUIRE_NO_FAIL(tester.Query("INSERT INTO test VALUES (NULL, 21)"));
	REQUIRE_NO_FAIL(tester.Query("INSERT INTO test VALUES (13, 22)"));

	// NULL selection
	result = tester.Query("SELECT a, b FROM test ORDER BY a");
	REQUIRE_NO_FAIL(*result);
	// NULL, 11, 13
	REQUIRE(result->IsNull(0, 0));
	REQUIRE(result->Fetch<int32_t>(0, 1) == 11);
	REQUIRE(result->Fetch<int32_t>(0, 2) == 13);
	// 21, 22, 22
	REQUIRE(result->Fetch<int32_t>(1, 0) == 21);
	REQUIRE(result->Fetch<int32_t>(1, 1) == 22);
	REQUIRE(result->Fetch<int32_t>(1, 2) == 22);

	REQUIRE(result->ColumnName(0) == "a");
	REQUIRE(result->ColumnName(1) == "b");
	REQUIRE(result->ColumnName(2) == "");
}

TEST_CASE("Test different types of C API", "[capi]") {
	CAPITester tester;
	unique_ptr<CAPIResult> result;

	// open the database in in-memory mode
	REQUIRE(tester.OpenDatabase(nullptr));

	// integer columns
	vector<string> types = {"TINYINT", "SMALLINT", "INTEGER", "BIGINT"};
	for (auto &type : types) {
		// create the table and insert values
		REQUIRE_NO_FAIL(tester.Query("BEGIN TRANSACTION"));
		REQUIRE_NO_FAIL(tester.Query("CREATE TABLE integers(i " + type + ")"));
		REQUIRE_NO_FAIL(tester.Query("INSERT INTO integers VALUES (1), (NULL)"));

		result = tester.Query("SELECT * FROM integers ORDER BY i");
		REQUIRE_NO_FAIL(*result);
		REQUIRE(result->IsNull(0, 0));
		REQUIRE(result->Fetch<int8_t>(0, 0) == 0);
		REQUIRE(result->Fetch<int16_t>(0, 0) == 0);
		REQUIRE(result->Fetch<int32_t>(0, 0) == 0);
		REQUIRE(result->Fetch<int64_t>(0, 0) == 0);
		REQUIRE(ApproxEqual(result->Fetch<float>(0, 0), 0.0f));
		REQUIRE(ApproxEqual(result->Fetch<double>(0, 0), 0.0));

		REQUIRE(!result->IsNull(0, 1));
		REQUIRE(result->Fetch<int8_t>(0, 1) == 1);
		REQUIRE(result->Fetch<int16_t>(0, 1) == 1);
		REQUIRE(result->Fetch<int32_t>(0, 1) == 1);
		REQUIRE(result->Fetch<int64_t>(0, 1) == 1);
		REQUIRE(ApproxEqual(result->Fetch<float>(0, 1), 1.0f));
		REQUIRE(ApproxEqual(result->Fetch<double>(0, 1), 1.0));
		REQUIRE(result->Fetch<string>(0, 1) == "1");

		REQUIRE_NO_FAIL(tester.Query("ROLLBACK"));
	}
	// real/double columns
	types = {"REAL", "DOUBLE"};
	for (auto &type : types) {
		// create the table and insert values
		REQUIRE_NO_FAIL(tester.Query("BEGIN TRANSACTION"));
		REQUIRE_NO_FAIL(tester.Query("CREATE TABLE doubles(i " + type + ")"));
		REQUIRE_NO_FAIL(tester.Query("INSERT INTO doubles VALUES (1), (NULL)"));

		result = tester.Query("SELECT * FROM doubles ORDER BY i");
		REQUIRE_NO_FAIL(*result);
		REQUIRE(result->IsNull(0, 0));
		REQUIRE(result->Fetch<int8_t>(0, 0) == 0);
		REQUIRE(result->Fetch<int16_t>(0, 0) == 0);
		REQUIRE(result->Fetch<int32_t>(0, 0) == 0);
		REQUIRE(result->Fetch<int64_t>(0, 0) == 0);
		REQUIRE(ApproxEqual(result->Fetch<float>(0, 0), 0.0f));
		REQUIRE(ApproxEqual(result->Fetch<double>(0, 0), 0.0));

		REQUIRE(!result->IsNull(0, 1));
		REQUIRE(result->Fetch<int8_t>(0, 1) == 1);
		REQUIRE(result->Fetch<int16_t>(0, 1) == 1);
		REQUIRE(result->Fetch<int32_t>(0, 1) == 1);
		REQUIRE(result->Fetch<int64_t>(0, 1) == 1);
		REQUIRE(ApproxEqual(result->Fetch<float>(0, 1), 1.0f));
		REQUIRE(ApproxEqual(result->Fetch<double>(0, 1), 1.0));

		REQUIRE_NO_FAIL(tester.Query("ROLLBACK"));
	}
	// date columns
	REQUIRE_NO_FAIL(tester.Query("CREATE TABLE dates(d DATE)"));
	REQUIRE_NO_FAIL(tester.Query("INSERT INTO dates VALUES ('1992-09-20'), (NULL), ('30000-09-20')"));

	result = tester.Query("SELECT * FROM dates ORDER BY d");
	REQUIRE_NO_FAIL(*result);
	REQUIRE(result->IsNull(0, 0));
	guinsoodb_date date = result->Fetch<guinsoodb_date>(0, 1);
	REQUIRE(date.year == 1992);
	REQUIRE(date.month == 9);
	REQUIRE(date.day == 20);
	REQUIRE(result->Fetch<string>(0, 1) == Value::DATE(1992, 9, 20).ToString());
	date = result->Fetch<guinsoodb_date>(0, 2);
	REQUIRE(date.year == 30000);
	REQUIRE(date.month == 9);
	REQUIRE(date.day == 20);
	REQUIRE(result->Fetch<string>(0, 2) == Value::DATE(30000, 9, 20).ToString());

	// timestamp columns
	REQUIRE_NO_FAIL(tester.Query("CREATE TABLE timestamps(t TIMESTAMP)"));
	REQUIRE_NO_FAIL(tester.Query("INSERT INTO timestamps VALUES ('1992-09-20 12:01:30'), (NULL)"));

	result = tester.Query("SELECT * FROM timestamps ORDER BY t");
	REQUIRE_NO_FAIL(*result);
	REQUIRE(result->IsNull(0, 0));
	guinsoodb_timestamp stamp = result->Fetch<guinsoodb_timestamp>(0, 1);
	REQUIRE(stamp.date.year == 1992);
	REQUIRE(stamp.date.month == 9);
	REQUIRE(stamp.date.day == 20);
	REQUIRE(stamp.time.hour == 12);
	REQUIRE(stamp.time.min == 1);
	REQUIRE(stamp.time.sec == 30);
	REQUIRE(stamp.time.micros == 0);
	REQUIRE(result->Fetch<string>(0, 1) == Value::TIMESTAMP(1992, 9, 20, 12, 1, 30, 0).ToString());

	// blob columns
	REQUIRE_NO_FAIL(tester.Query("CREATE TABLE blobs(b BLOB)"));
	REQUIRE_NO_FAIL(tester.Query("INSERT INTO blobs VALUES ('hello\\x12world'), ('\\x00'), (NULL)"));

	result = tester.Query("SELECT * FROM blobs");
	REQUIRE_NO_FAIL(*result);
	REQUIRE(!result->IsNull(0, 0));
	guinsoodb_blob blob = result->Fetch<guinsoodb_blob>(0, 0);
	REQUIRE(blob.size == 11);
	REQUIRE(memcmp(blob.data, "hello\012world", 11));
	REQUIRE(result->Fetch<string>(0, 1) == "\\x00");
	REQUIRE(result->IsNull(0, 2));
	blob = result->Fetch<guinsoodb_blob>(0, 2);
	REQUIRE(blob.data == nullptr);
	REQUIRE(blob.size == 0);

	// boolean columns
	REQUIRE_NO_FAIL(tester.Query("CREATE TABLE booleans(b BOOLEAN)"));
	REQUIRE_NO_FAIL(tester.Query("INSERT INTO booleans VALUES (42 > 60), (42 > 20), (42 > NULL)"));

	result = tester.Query("SELECT * FROM booleans ORDER BY b");
	REQUIRE_NO_FAIL(*result);
	REQUIRE(result->IsNull(0, 0));
	REQUIRE(!result->Fetch<bool>(0, 0));
	REQUIRE(!result->Fetch<bool>(0, 1));
	REQUIRE(result->Fetch<bool>(0, 2));
	REQUIRE(result->Fetch<string>(0, 2) == Value::BOOLEAN(true).ToString());
}

TEST_CASE("Test errors in C API", "[capi]") {
	CAPITester tester;
	unique_ptr<CAPIResult> result;

	// cannot open database in random directory
	REQUIRE(!tester.OpenDatabase("/bla/this/directory/should/not/exist/hopefully/awerar333"));
	REQUIRE(tester.OpenDatabase(nullptr));

	// syntax error in query
	REQUIRE_FAIL(tester.Query("SELEC * FROM TABLE"));
	// bind error
	REQUIRE_FAIL(tester.Query("SELECT * FROM TABLE"));

	guinsoodb_result res;
	guinsoodb_prepared_statement stmt = nullptr;
	// fail prepare API calls
	REQUIRE(guinsoodb_prepare(NULL, "SELECT 42", &stmt) == GuinsooDBError);
	REQUIRE(guinsoodb_prepare(tester.connection, NULL, &stmt) == GuinsooDBError);
	REQUIRE(guinsoodb_bind_boolean(NULL, 0, true) == GuinsooDBError);
	REQUIRE(guinsoodb_execute_prepared(NULL, &res) == GuinsooDBError);
	guinsoodb_destroy_prepare(NULL);
}

TEST_CASE("Test prepared statements in C API", "[capi][.]") {
	CAPITester tester;
	unique_ptr<CAPIResult> result;
	guinsoodb_result res;
	guinsoodb_prepared_statement stmt = nullptr;
	guinsoodb_state status;

	// open the database in in-memory mode
	REQUIRE(tester.OpenDatabase(nullptr));

	status = guinsoodb_prepare(tester.connection, "SELECT CAST($1 AS BIGINT)", &stmt);
	REQUIRE(status == GuinsooDBSuccess);
	REQUIRE(stmt != nullptr);

	status = guinsoodb_bind_boolean(stmt, 1, 1);
	REQUIRE(status == GuinsooDBSuccess);
	status = guinsoodb_bind_boolean(stmt, 2, 1);
	REQUIRE(status == GuinsooDBError);

	status = guinsoodb_execute_prepared(stmt, &res);
	REQUIRE(status == GuinsooDBSuccess);
	REQUIRE(guinsoodb_value_int64(&res, 0, 0) == 1);
	guinsoodb_destroy_result(&res);

	guinsoodb_bind_int8(stmt, 1, 8);
	status = guinsoodb_execute_prepared(stmt, &res);
	REQUIRE(status == GuinsooDBSuccess);
	REQUIRE(guinsoodb_value_int64(&res, 0, 0) == 8);
	guinsoodb_destroy_result(&res);

	guinsoodb_bind_int16(stmt, 1, 16);
	status = guinsoodb_execute_prepared(stmt, &res);
	REQUIRE(status == GuinsooDBSuccess);
	REQUIRE(guinsoodb_value_int64(&res, 0, 0) == 16);
	guinsoodb_destroy_result(&res);

	guinsoodb_bind_int32(stmt, 1, 32);
	status = guinsoodb_execute_prepared(stmt, &res);
	REQUIRE(status == GuinsooDBSuccess);
	REQUIRE(guinsoodb_value_int64(&res, 0, 0) == 32);
	guinsoodb_destroy_result(&res);

	guinsoodb_bind_int64(stmt, 1, 64);
	status = guinsoodb_execute_prepared(stmt, &res);
	REQUIRE(status == GuinsooDBSuccess);
	REQUIRE(guinsoodb_value_int64(&res, 0, 0) == 64);
	guinsoodb_destroy_result(&res);

	guinsoodb_bind_float(stmt, 1, 42.0);
	status = guinsoodb_execute_prepared(stmt, &res);
	REQUIRE(status == GuinsooDBSuccess);
	REQUIRE(guinsoodb_value_int64(&res, 0, 0) == 42);
	guinsoodb_destroy_result(&res);

	guinsoodb_bind_double(stmt, 1, 43.0);
	status = guinsoodb_execute_prepared(stmt, &res);
	REQUIRE(status == GuinsooDBSuccess);
	REQUIRE(guinsoodb_value_int64(&res, 0, 0) == 43);
	guinsoodb_destroy_result(&res);

	guinsoodb_bind_varchar(stmt, 1, "44");
	status = guinsoodb_execute_prepared(stmt, &res);
	REQUIRE(status == GuinsooDBSuccess);
	REQUIRE(guinsoodb_value_int64(&res, 0, 0) == 44);
	guinsoodb_destroy_result(&res);

	guinsoodb_bind_null(stmt, 1);
	status = guinsoodb_execute_prepared(stmt, &res);
	REQUIRE(status == GuinsooDBSuccess);
	REQUIRE(res.columns[0].nullmask[0] == true);
	guinsoodb_destroy_result(&res);

	guinsoodb_destroy_prepare(&stmt);
	// again to make sure it does not crash
	guinsoodb_destroy_result(&res);
	guinsoodb_destroy_prepare(&stmt);

	status = guinsoodb_prepare(tester.connection, "SELECT CAST($1 AS VARCHAR)", &stmt);
	REQUIRE(status == GuinsooDBSuccess);
	REQUIRE(stmt != nullptr);

	guinsoodb_bind_varchar_length(stmt, 1, "hello world", 5);
	status = guinsoodb_execute_prepared(stmt, &res);
	REQUIRE(status == GuinsooDBSuccess);
	auto value = guinsoodb_value_varchar(&res, 0, 0);
	REQUIRE(string(value) == "hello");
	free(value);
	guinsoodb_destroy_result(&res);

	guinsoodb_bind_blob(stmt, 1, "hello\0world", 11);
	status = guinsoodb_execute_prepared(stmt, &res);
	REQUIRE(status == GuinsooDBSuccess);
	value = guinsoodb_value_varchar(&res, 0, 0);
	REQUIRE(string(value) == "hello\\x00world");
	free(value);
	guinsoodb_destroy_result(&res);

	guinsoodb_destroy_prepare(&stmt);

	status = guinsoodb_query(tester.connection, "CREATE TABLE a (i INTEGER)", NULL);
	REQUIRE(status == GuinsooDBSuccess);

	status = guinsoodb_prepare(tester.connection, "INSERT INTO a VALUES (?)", &stmt);
	REQUIRE(status == GuinsooDBSuccess);
	REQUIRE(stmt != nullptr);
	idx_t nparams;
	REQUIRE(guinsoodb_nparams(stmt, &nparams) == GuinsooDBSuccess);
	REQUIRE(nparams == 1);

	for (int32_t i = 1; i <= 1000; i++) {
		guinsoodb_bind_int32(stmt, 1, i);
		status = guinsoodb_execute_prepared(stmt, nullptr);
		REQUIRE(status == GuinsooDBSuccess);
	}
	guinsoodb_destroy_prepare(&stmt);

	status = guinsoodb_prepare(tester.connection, "SELECT SUM(i)*$1-$2 FROM a", &stmt);
	REQUIRE(status == GuinsooDBSuccess);
	REQUIRE(stmt != nullptr);
	guinsoodb_bind_int32(stmt, 1, 2);
	guinsoodb_bind_int32(stmt, 2, 1000);

	status = guinsoodb_execute_prepared(stmt, &res);
	REQUIRE(status == GuinsooDBSuccess);
	REQUIRE(guinsoodb_value_int32(&res, 0, 0) == 1000000);
	guinsoodb_destroy_result(&res);
	guinsoodb_destroy_prepare(&stmt);

	// not-so-happy path
	status = guinsoodb_prepare(tester.connection, "SELECT XXXXX", &stmt);
	REQUIRE(status == GuinsooDBError);
	guinsoodb_destroy_prepare(&stmt);

	status = guinsoodb_prepare(tester.connection, "SELECT CAST($1 AS INTEGER)", &stmt);
	REQUIRE(status == GuinsooDBSuccess);
	REQUIRE(stmt != nullptr);

	status = guinsoodb_execute_prepared(stmt, &res);
	REQUIRE(status == GuinsooDBError);
	guinsoodb_destroy_result(&res);
	guinsoodb_destroy_prepare(&stmt);
}

TEST_CASE("Test appender statements in C API", "[capi][.]") {
	CAPITester tester;
	unique_ptr<CAPIResult> result;
	guinsoodb_state status;

	// open the database in in-memory mode
	REQUIRE(tester.OpenDatabase(nullptr));

	tester.Query("CREATE TABLE test (i INTEGER, d double, s string)");
	guinsoodb_appender appender;

	status = guinsoodb_appender_create(tester.connection, nullptr, "test", nullptr);
	REQUIRE(status == GuinsooDBError);

	status = guinsoodb_appender_create(tester.connection, nullptr, "test", &appender);
	REQUIRE(status == GuinsooDBSuccess);

	status = guinsoodb_appender_begin_row(appender);
	REQUIRE(status == GuinsooDBSuccess);

	status = guinsoodb_append_int32(appender, 42);
	REQUIRE(status == GuinsooDBSuccess);

	status = guinsoodb_append_double(appender, 4.2);
	REQUIRE(status == GuinsooDBSuccess);

	status = guinsoodb_append_varchar(appender, "Hello, World");
	REQUIRE(status == GuinsooDBSuccess);

	// out of cols here
	status = guinsoodb_append_int32(appender, 42);
	REQUIRE(status == GuinsooDBError);

	status = guinsoodb_appender_end_row(appender);
	REQUIRE(status == GuinsooDBSuccess);

	status = guinsoodb_appender_flush(appender);
	REQUIRE(status == GuinsooDBSuccess);

	// we can flush again why not
	status = guinsoodb_appender_flush(appender);
	REQUIRE(status == GuinsooDBSuccess);

	status = guinsoodb_appender_close(appender);
	REQUIRE(status == GuinsooDBSuccess);

	result = tester.Query("SELECT * FROM test");
	REQUIRE_NO_FAIL(*result);
	REQUIRE(result->Fetch<int32_t>(0, 0) == 42);
	REQUIRE(result->Fetch<double>(1, 0) == 4.2);
	REQUIRE(result->Fetch<string>(2, 0) == "Hello, World");

	status = guinsoodb_appender_destroy(&appender);
	REQUIRE(status == GuinsooDBSuccess);

	// this has been destroyed

	status = guinsoodb_appender_close(appender);
	REQUIRE(status == GuinsooDBError);

	status = guinsoodb_appender_flush(appender);
	REQUIRE(status == GuinsooDBError);

	status = guinsoodb_appender_begin_row(appender);
	REQUIRE(status == GuinsooDBError);

	status = guinsoodb_appender_end_row(appender);
	REQUIRE(status == GuinsooDBError);

	status = guinsoodb_append_int32(appender, 42);
	REQUIRE(status == GuinsooDBError);

	status = guinsoodb_appender_destroy(&appender);
	REQUIRE(status == GuinsooDBError);

	status = guinsoodb_appender_close(nullptr);
	REQUIRE(status == GuinsooDBError);

	status = guinsoodb_appender_flush(nullptr);
	REQUIRE(status == GuinsooDBError);

	status = guinsoodb_appender_begin_row(nullptr);
	REQUIRE(status == GuinsooDBError);

	status = guinsoodb_appender_end_row(nullptr);
	REQUIRE(status == GuinsooDBError);

	status = guinsoodb_append_int32(nullptr, 42);
	REQUIRE(status == GuinsooDBError);

	status = guinsoodb_appender_destroy(nullptr);
	REQUIRE(status == GuinsooDBError);

	status = guinsoodb_appender_destroy(nullptr);
	REQUIRE(status == GuinsooDBError);
}

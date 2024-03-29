#include "catch.hpp"
#include "test_helpers.hpp"
#include "guinsoodb/common/types/date.hpp"
#include "guinsoodb/common/types/time.hpp"
#include "guinsoodb/common/types/timestamp.hpp"

using namespace guinsoodb;
using namespace std;

TEST_CASE("Test results API", "[api]") {
	GuinsooDB db(nullptr);
	Connection con(db);

	// result equality
	auto result = con.Query("SELECT 42");
	auto result2 = con.Query("SELECT 42");
	REQUIRE(result->Equals(*result2));

	// result inequality
	result = con.Query("SELECT 42");
	result2 = con.Query("SELECT 43");
	REQUIRE(!result->Equals(*result2));

	// stream query to string
	auto stream_result = con.SendQuery("SELECT 42");
	auto str = stream_result->ToString();
	REQUIRE(!str.empty());

	// materialized query to string
	result = con.Query("SELECT 42");
	str = result->ToString();
	REQUIRE(!str.empty());

	// error to string
	result = con.Query("SELEC 42");
	str = result->ToString();
	REQUIRE(!str.empty());
}

TEST_CASE("Test iterating over results", "[api]") {
	GuinsooDB db(nullptr);
	Connection con(db);

	REQUIRE_NO_FAIL(con.Query("CREATE TABLE data(i INTEGER, j VARCHAR)"));
	REQUIRE_NO_FAIL(con.Query("INSERT INTO data VALUES (1, 'hello'), (2, 'test')"));

	vector<int> i_values = {1, 2};
	vector<string> j_values = {"hello", "test"};
	idx_t row_count = 0;
	auto result = con.Query("SELECT * FROM data;");
	for (auto &row : *result) {
		REQUIRE(row.GetValue<int>(0) == i_values[row.row]);
		REQUIRE(row.GetValue<string>(1) == j_values[row.row]);
		row_count++;
	}
	REQUIRE(row_count == 2);
}

TEST_CASE("Test different result types", "[api]") {
	GuinsooDB db(nullptr);
	Connection con(db);

	REQUIRE_NO_FAIL(
	    con.Query("CREATE TABLE data(i INTEGER, j VARCHAR, k DECIMAL(38,1), l DECIMAL(18,3), m HUGEINT, n DOUBLE)"));
	REQUIRE_NO_FAIL(con.Query("INSERT INTO data VALUES (23, '17.1', 94289, 9842, 4982412, 17.3)"));

	idx_t row_count = 0;
	auto result = con.Query("SELECT * FROM data;");
	for (auto &row : *result) {
		REQUIRE(row.GetValue<int>(0) == 23);
		REQUIRE(row.GetValue<int64_t>(0) == 23);
		REQUIRE(row.GetValue<double>(0) == 23);
		REQUIRE(row.GetValue<string>(0) == "23");

		REQUIRE(row.GetValue<int>(1) == 17);
		REQUIRE(row.GetValue<int64_t>(1) == 17);
		REQUIRE(row.GetValue<double>(1) == 17.1);
		REQUIRE(row.GetValue<string>(1) == "17.1");

		REQUIRE(row.GetValue<int>(2) == 94289);
		REQUIRE(row.GetValue<int64_t>(2) == 94289);
		REQUIRE(row.GetValue<double>(2) == 94289);

		REQUIRE(row.GetValue<int>(3) == 9842);
		REQUIRE(row.GetValue<int64_t>(3) == 9842);
		REQUIRE(row.GetValue<double>(3) == 9842);

		REQUIRE(row.GetValue<int>(4) == 4982412);
		REQUIRE(row.GetValue<int64_t>(4) == 4982412);
		REQUIRE(row.GetValue<double>(4) == 4982412);
		REQUIRE(row.GetValue<string>(4) == "4982412");

		REQUIRE(row.GetValue<int>(5) == 17);
		REQUIRE(row.GetValue<int64_t>(5) == 17);
		REQUIRE(row.GetValue<double>(5) == 17.3);

		row_count++;
	}
	REQUIRE(row_count == 1);
}

TEST_CASE("Test dates/times/timestamps", "[api]") {
	GuinsooDB db(nullptr);
	Connection con(db);

	REQUIRE_NO_FAIL(con.Query("CREATE TABLE data(i DATE, j TIME, k TIMESTAMP)"));
	REQUIRE_NO_FAIL(
	    con.Query("INSERT INTO data VALUES (DATE '1992-01-01', TIME '13:00:17', TIMESTAMP '1993-01-01 14:00:17')"));

	idx_t row_count = 0;
	auto result = con.Query("SELECT * FROM data;");
	for (auto &row : *result) {
		int32_t year, month, day;
		int32_t hour, minute, second, milisecond;

		auto date = row.GetValue<int32_t>(0);
		auto time = row.GetValue<int64_t>(1);
		auto timestamp = row.GetValue<int64_t>(2);
		Date::Convert(date, year, month, day);
		REQUIRE(year == 1992);
		REQUIRE(month == 1);
		REQUIRE(day == 1);

		Time::Convert(time, hour, minute, second, milisecond);
		REQUIRE(hour == 13);
		REQUIRE(minute == 0);
		REQUIRE(second == 17);
		REQUIRE(milisecond == 0);

		Timestamp::Convert(timestamp, date, time);
		Date::Convert(date, year, month, day);
		Time::Convert(time, hour, minute, second, milisecond);

		REQUIRE(year == 1993);
		REQUIRE(month == 1);
		REQUIRE(day == 1);
		REQUIRE(hour == 14);
		REQUIRE(minute == 0);
		REQUIRE(second == 17);
		REQUIRE(milisecond == 0);

		row_count++;
	}
	REQUIRE(row_count == 1);
}

TEST_CASE("Error in streaming result after initial query", "[api][.]") {
	GuinsooDB db(nullptr);
	Connection con(db);

	// create a big table with strings that are numbers
	REQUIRE_NO_FAIL(con.Query("CREATE TABLE strings(v VARCHAR)"));
	for (size_t i = 0; i < STANDARD_VECTOR_SIZE * 2 - 1; i++) {
		REQUIRE_NO_FAIL(con.Query("INSERT INTO strings VALUES ('" + to_string(i) + "')"));
	}
	// now insert one non-numeric value
	REQUIRE_NO_FAIL(con.Query("INSERT INTO strings VALUES ('hello')"));

	// now create a streaming result
	auto result = con.SendQuery("SELECT CAST(v AS INTEGER) FROM strings");
	REQUIRE_NO_FAIL(*result);
	// initial query does not fail!
	auto chunk = result->Fetch();
	REQUIRE(chunk);
	// but subsequent query fails!
	chunk = result->Fetch();
	REQUIRE(!chunk);
	REQUIRE(!result->success);
	auto str = result->ToString();
	REQUIRE(!str.empty());
}

#include "catch.hpp"
#include "guinsoodb/common/arrow.hpp"
#include "test_helpers.hpp"

using namespace guinsoodb;
using namespace std;

struct MyArrowArrayStream {
	MyArrowArrayStream(unique_ptr<QueryResult> guinsoodb_result) : guinsoodb_result(move(guinsoodb_result)) {
		stream.get_schema = MyArrowArrayStream::my_stream_getschema;
		stream.get_next = MyArrowArrayStream::my_stream_getnext;
		stream.release = MyArrowArrayStream::my_stream_release;
		stream.get_last_error = MyArrowArrayStream::my_stream_getlasterror;
		stream.private_data = this;
	}

	static int my_stream_getschema(struct ArrowArrayStream *stream, struct ArrowSchema *out) {
		D_ASSERT(stream->private_data);
		auto my_stream = (MyArrowArrayStream *)stream->private_data;
		if (!stream->release) {
			my_stream->last_error = "stream was released";
			return -1;
		}
		my_stream->guinsoodb_result->ToArrowSchema(out);
		return 0;
	}

	static int my_stream_getnext(struct ArrowArrayStream *stream, struct ArrowArray *out) {
		D_ASSERT(stream->private_data);
		auto my_stream = (MyArrowArrayStream *)stream->private_data;
		if (!stream->release) {
			my_stream->last_error = "stream was released";
			return -1;
		}
		auto chunk = my_stream->guinsoodb_result->Fetch();
		if (!chunk || chunk->size() == 0) {
			return 0;
		}
		chunk->ToArrowArray(out);
		return 0;
	}

	static void my_stream_release(struct ArrowArrayStream *stream) {
		if (!stream->release) {
			return;
		}
		stream->release = nullptr;
		delete (MyArrowArrayStream *)stream->private_data;
	}

	static const char *my_stream_getlasterror(struct ArrowArrayStream *stream) {
		if (!stream->release) {
			return "stream was released";
		}
		D_ASSERT(stream->private_data);
		auto my_stream = (MyArrowArrayStream *)stream->private_data;
		return my_stream->last_error.c_str();
	}

	unique_ptr<QueryResult> guinsoodb_result;
	ArrowArrayStream stream;
	string last_error;
};

static void test_arrow_round_trip(string q) {
	GuinsooDB db(nullptr);
	Connection con(db);

	// query that creates a bunch of values across the types
	auto result = con.Query(q);
	REQUIRE(result->success);
	auto my_stream = new MyArrowArrayStream(move(result));
	auto result2 = con.TableFunction("arrow_scan", {Value::POINTER((uintptr_t)&my_stream->stream)})->Execute();

	idx_t column_count = result2->ColumnCount();
	vector<vector<Value>> values;
	values.resize(column_count);
	while (true) {
		auto chunk = result2->Fetch();
		if (!chunk || chunk->size() == 0) {
			break;
		}
		for (idx_t c = 0; c < column_count; c++) {
			for (idx_t r = 0; r < chunk->size(); r++) {
				values[c].push_back(chunk->GetValue(c, r));
			}
		}
	}
	auto original_result = con.Query(q);
	for (idx_t c = 0; c < column_count; c++) {
		REQUIRE(CHECK_COLUMN(*original_result, c, values[c]));
	}
}

TEST_CASE("Test Arrow API round trip", "[arrow]") {
	// many types
	test_arrow_round_trip(
	    "select NULL c_null, (c % 4 = 0)::bool c_bool, (c%128)::tinyint c_tinyint, c::smallint*1000 c_smallint, "
	    "c::integer*100000 c_integer, c::bigint*1000000000000 c_bigint, c::hugeint*10000000000000000000000000000000 "
	    "c_hugeint, c::float c_float, c::double c_double, 'c_' || c::string c_string, DATE '1992-01-01'::date c_date, "
	    "'1969-01-01'::date, TIME '13:07:16'::time c_time, timestamp '1992-01-01 12:00:00' c_timestamp "
	    "from (select case when range % 2 == 0 then range else null end as c from range(-10, 10)) sq");
	// big result set
	test_arrow_round_trip("select i from range(0, 2000) sq(i)");
}

TEST_CASE("Test Arrow API unsigned", "[arrow]") {
	// all unsigned types
	test_arrow_round_trip(
	    "select (c%128)::utinyint c_utinyint ,c::usmallint*1000 c_usmallint, "
	    "c::uinteger*100000 c_uinteger, c::ubigint*1000000000000 c_ubigint from (select case when range "
	    "% 2 == 0 then range else null end as c from range(0, 100)) sq");
	// big result set
	test_arrow_round_trip("select i from range(0, 2000) sq(i)");
}
// TODO interval decimal

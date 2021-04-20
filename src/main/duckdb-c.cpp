#include "guinsoodb/common/types/date.hpp"
#include "guinsoodb/common/types/time.hpp"
#include "guinsoodb/common/types/timestamp.hpp"
#include "guinsoodb/common/vector_operations/vector_operations.hpp"
#include "guinsoodb/main/appender.hpp"

#include "guinsoodb.h"
#include "guinsoodb.hpp"

#include <cstring>
#include <cassert>

#ifdef _WIN32
#define strdup _strdup
#endif

#ifdef GetCValue
#undef GetCValue
#endif

using namespace guinsoodb;

static guinsoodb_type ConvertCPPTypeToC(LogicalType type);
static idx_t GetCTypeSize(guinsoodb_type type);
namespace guinsoodb {
struct DatabaseData {
	DatabaseData() : database(nullptr) {
	}
	~DatabaseData() {
		if (database) {
			delete database;
		}
	}

	GuinsooDB *database;
};
} // namespace guinsoodb
guinsoodb_state guinsoodb_open(const char *path, guinsoodb_database *out) {
	auto wrapper = new DatabaseData();
	try {
		wrapper->database = new GuinsooDB(path);
	} catch (...) {
		delete wrapper;
		return GuinsooDBError;
	}
	*out = (guinsoodb_database)wrapper;
	return GuinsooDBSuccess;
}

void guinsoodb_close(guinsoodb_database *database) {
	if (*database) {
		auto wrapper = (DatabaseData *)*database;
		delete wrapper;
		*database = nullptr;
	}
}

guinsoodb_state guinsoodb_connect(guinsoodb_database database, guinsoodb_connection *out) {
	auto wrapper = (DatabaseData *)database;
	Connection *connection;
	try {
		connection = new Connection(*wrapper->database);
	} catch (...) {
		return GuinsooDBError;
	}
	*out = (guinsoodb_connection)connection;
	return GuinsooDBSuccess;
}

void guinsoodb_disconnect(guinsoodb_connection *connection) {
	if (*connection) {
		Connection *conn = (Connection *)*connection;
		delete conn;
		*connection = nullptr;
	}
}

template <class T>
void WriteData(guinsoodb_result *out, ChunkCollection &source, idx_t col) {
	idx_t row = 0;
	auto target = (T *)out->columns[col].data;
	for (auto &chunk : source.Chunks()) {
		auto source = FlatVector::GetData<T>(chunk->data[col]);
		auto &mask = FlatVector::Validity(chunk->data[col]);

		for (idx_t k = 0; k < chunk->size(); k++, row++) {
			if (!mask.RowIsValid(k)) {
				continue;
			}
			target[row] = source[k];
		}
	}
}

static guinsoodb_state guinsoodb_translate_result(MaterializedQueryResult *result, guinsoodb_result *out) {
	D_ASSERT(result);
	if (!out) {
		// no result to write to, only return the status
		return result->success ? GuinsooDBSuccess : GuinsooDBError;
	}
	out->error_message = nullptr;
	if (!result->success) {
		// write the error message
		out->error_message = strdup(result->error.c_str());
		return GuinsooDBError;
	}
	// copy the data
	// first write the meta data
	out->column_count = result->types.size();
	out->row_count = result->collection.Count();
	out->columns = (guinsoodb_column *)malloc(sizeof(guinsoodb_column) * out->column_count);
	if (!out->columns) {
		return GuinsooDBError;
	}
	// zero initialize the columns (so we can cleanly delete it in case a malloc fails)
	memset(out->columns, 0, sizeof(guinsoodb_column) * out->column_count);
	for (idx_t i = 0; i < out->column_count; i++) {
		out->columns[i].type = ConvertCPPTypeToC(result->types[i]);
		out->columns[i].name = strdup(result->names[i].c_str());
		out->columns[i].nullmask = (bool *)malloc(sizeof(bool) * out->row_count);
		out->columns[i].data = malloc(GetCTypeSize(out->columns[i].type) * out->row_count);
		if (!out->columns[i].nullmask || !out->columns[i].name || !out->columns[i].data) {
			// malloc failure
			return GuinsooDBError;
		}
		// memset data to 0 for VARCHAR columns for safe deletion later
		if (result->types[i].InternalType() == PhysicalType::VARCHAR) {
			memset(out->columns[i].data, 0, GetCTypeSize(out->columns[i].type) * out->row_count);
		}
	}
	// now write the data
	for (idx_t col = 0; col < out->column_count; col++) {
		// first set the nullmask
		idx_t row = 0;
		for (auto &chunk : result->collection.Chunks()) {
			for (idx_t k = 0; k < chunk->size(); k++) {
				out->columns[col].nullmask[row++] = FlatVector::IsNull(chunk->data[col], k);
			}
		}
		// then write the data
		switch (result->types[col].id()) {
		case LogicalTypeId::BOOLEAN:
			WriteData<bool>(out, result->collection, col);
			break;
		case LogicalTypeId::TINYINT:
			WriteData<int8_t>(out, result->collection, col);
			break;
		case LogicalTypeId::SMALLINT:
			WriteData<int16_t>(out, result->collection, col);
			break;
		case LogicalTypeId::INTEGER:
			WriteData<int32_t>(out, result->collection, col);
			break;
		case LogicalTypeId::BIGINT:
			WriteData<int64_t>(out, result->collection, col);
			break;
		case LogicalTypeId::FLOAT:
			WriteData<float>(out, result->collection, col);
			break;
		case LogicalTypeId::DOUBLE:
			WriteData<double>(out, result->collection, col);
			break;
		case LogicalTypeId::VARCHAR: {
			idx_t row = 0;
			auto target = (const char **)out->columns[col].data;
			for (auto &chunk : result->collection.Chunks()) {
				auto source = FlatVector::GetData<string_t>(chunk->data[col]);
				for (idx_t k = 0; k < chunk->size(); k++) {
					if (!FlatVector::IsNull(chunk->data[col], k)) {
						target[row] = (char *)malloc(source[k].GetSize() + 1);
						assert(target[row]);
						memcpy((void *)target[row], source[k].GetDataUnsafe(), source[k].GetSize());
						auto write_arr = (char *)target[row];
						write_arr[source[k].GetSize()] = '\0';
					}
					row++;
				}
			}
			break;
		}
		case LogicalTypeId::BLOB: {
			idx_t row = 0;
			auto target = (guinsoodb_blob *)out->columns[col].data;
			for (auto &chunk : result->collection.Chunks()) {
				auto source = FlatVector::GetData<string_t>(chunk->data[col]);
				for (idx_t k = 0; k < chunk->size(); k++) {
					if (!FlatVector::IsNull(chunk->data[col], k)) {
						target[row].data = (char *)malloc(source[k].GetSize());
						target[row].size = source[k].GetSize();
						assert(target[row].data);
						memcpy((void *)target[row].data, source[k].GetDataUnsafe(), source[k].GetSize());
					}
					row++;
				}
			}
			break;
		}
		case LogicalTypeId::DATE: {
			idx_t row = 0;
			auto target = (guinsoodb_date *)out->columns[col].data;
			for (auto &chunk : result->collection.Chunks()) {
				auto source = FlatVector::GetData<date_t>(chunk->data[col]);
				for (idx_t k = 0; k < chunk->size(); k++) {
					if (!FlatVector::IsNull(chunk->data[col], k)) {
						int32_t year, month, day;
						Date::Convert(source[k], year, month, day);
						target[row].year = year;
						target[row].month = month;
						target[row].day = day;
					}
					row++;
				}
			}
			break;
		}
		case LogicalTypeId::TIME: {
			idx_t row = 0;
			auto target = (guinsoodb_time *)out->columns[col].data;
			for (auto &chunk : result->collection.Chunks()) {
				auto source = FlatVector::GetData<dtime_t>(chunk->data[col]);
				for (idx_t k = 0; k < chunk->size(); k++) {
					if (!FlatVector::IsNull(chunk->data[col], k)) {
						int32_t hour, min, sec, micros;
						Time::Convert(source[k], hour, min, sec, micros);
						target[row].hour = hour;
						target[row].min = min;
						target[row].sec = sec;
						target[row].micros = micros;
					}
					row++;
				}
			}
			break;
		}
		case LogicalTypeId::TIMESTAMP: {
			idx_t row = 0;
			auto target = (guinsoodb_timestamp *)out->columns[col].data;
			for (auto &chunk : result->collection.Chunks()) {
				auto source = FlatVector::GetData<timestamp_t>(chunk->data[col]);
				for (idx_t k = 0; k < chunk->size(); k++) {
					if (!FlatVector::IsNull(chunk->data[col], k)) {
						date_t date;
						dtime_t time;
						Timestamp::Convert(source[k], date, time);

						int32_t year, month, day;
						Date::Convert(date, year, month, day);

						int32_t hour, min, sec, micros;
						Time::Convert(time, hour, min, sec, micros);

						target[row].date.year = year;
						target[row].date.month = month;
						target[row].date.day = day;
						target[row].time.hour = hour;
						target[row].time.min = min;
						target[row].time.sec = sec;
						target[row].time.micros = micros;
					}
					row++;
				}
			}
			break;
		}
		case LogicalTypeId::HUGEINT: {
			idx_t row = 0;
			auto target = (guinsoodb_hugeint *)out->columns[col].data;
			for (auto &chunk : result->collection.Chunks()) {
				auto source = FlatVector::GetData<hugeint_t>(chunk->data[col]);
				for (idx_t k = 0; k < chunk->size(); k++) {
					if (!FlatVector::IsNull(chunk->data[col], k)) {
						target[row].lower = source[k].lower;
						target[row].upper = source[k].upper;
					}
					row++;
				}
			}
			break;
		}
		case LogicalTypeId::INTERVAL: {
			idx_t row = 0;
			auto target = (guinsoodb_interval *)out->columns[col].data;
			for (auto &chunk : result->collection.Chunks()) {
				auto source = FlatVector::GetData<interval_t>(chunk->data[col]);
				for (idx_t k = 0; k < chunk->size(); k++) {
					if (!FlatVector::IsNull(chunk->data[col], k)) {
						target[row].days = source[k].days;
						target[row].months = source[k].months;
						target[row].micros = source[k].micros;
					}
					row++;
				}
			}
			break;
		}
		default:
			// unsupported type for C API
			D_ASSERT(0);
			return GuinsooDBError;
		}
	}
	return GuinsooDBSuccess;
}

guinsoodb_state guinsoodb_query(guinsoodb_connection connection, const char *query, guinsoodb_result *out) {
	Connection *conn = (Connection *)connection;
	auto result = conn->Query(query);
	return guinsoodb_translate_result(result.get(), out);
}

static void guinsoodb_destroy_column(guinsoodb_column column, idx_t count) {
	if (column.data) {
		if (column.type == GUINSOODB_TYPE_VARCHAR) {
			// varchar, delete individual strings
			auto data = (char **)column.data;
			for (idx_t i = 0; i < count; i++) {
				if (data[i]) {
					free(data[i]);
				}
			}
		} else if (column.type == GUINSOODB_TYPE_BLOB) {
			// blob, delete individual blobs
			auto data = (guinsoodb_blob *)column.data;
			for (idx_t i = 0; i < count; i++) {
				if (data[i].data) {
					free((void *)data[i].data);
				}
			}
		}
		free(column.data);
	}
	if (column.nullmask) {
		free(column.nullmask);
	}
	if (column.name) {
		free(column.name);
	}
}

void guinsoodb_destroy_result(guinsoodb_result *result) {
	if (result->error_message) {
		free(result->error_message);
	}
	if (result->columns) {
		for (idx_t i = 0; i < result->column_count; i++) {
			guinsoodb_destroy_column(result->columns[i], result->row_count);
		}
		free(result->columns);
	}
	memset(result, 0, sizeof(guinsoodb_result));
}
namespace guinsoodb {
struct PreparedStatementWrapper {
	PreparedStatementWrapper() : statement(nullptr) {
	}
	~PreparedStatementWrapper() {
	}
	unique_ptr<PreparedStatement> statement;
	vector<Value> values;
};
} // namespace guinsoodb
guinsoodb_state guinsoodb_prepare(guinsoodb_connection connection, const char *query,
                            guinsoodb_prepared_statement *out_prepared_statement) {
	if (!connection || !query) {
		return GuinsooDBError;
	}
	auto wrapper = new PreparedStatementWrapper();
	Connection *conn = (Connection *)connection;
	wrapper->statement = conn->Prepare(query);
	*out_prepared_statement = (guinsoodb_prepared_statement)wrapper;
	return wrapper->statement->success ? GuinsooDBSuccess : GuinsooDBError;
}

guinsoodb_state guinsoodb_nparams(guinsoodb_prepared_statement prepared_statement, idx_t *nparams_out) {
	auto wrapper = (PreparedStatementWrapper *)prepared_statement;
	if (!wrapper || !wrapper->statement || !wrapper->statement->success) {
		return GuinsooDBError;
	}
	*nparams_out = wrapper->statement->n_param;
	return GuinsooDBSuccess;
}

static guinsoodb_state guinsoodb_bind_value(guinsoodb_prepared_statement prepared_statement, idx_t param_idx, Value val) {
	auto wrapper = (PreparedStatementWrapper *)prepared_statement;
	if (!wrapper || !wrapper->statement || !wrapper->statement->success) {
		return GuinsooDBError;
	}
	if (param_idx > wrapper->statement->n_param) {
		return GuinsooDBError;
	}
	if (param_idx > wrapper->values.size()) {
		wrapper->values.resize(param_idx);
	}
	wrapper->values[param_idx - 1] = val;
	return GuinsooDBSuccess;
}

guinsoodb_state guinsoodb_bind_boolean(guinsoodb_prepared_statement prepared_statement, idx_t param_idx, bool val) {
	return guinsoodb_bind_value(prepared_statement, param_idx, Value::BOOLEAN(val));
}

guinsoodb_state guinsoodb_bind_int8(guinsoodb_prepared_statement prepared_statement, idx_t param_idx, int8_t val) {
	return guinsoodb_bind_value(prepared_statement, param_idx, Value::TINYINT(val));
}

guinsoodb_state guinsoodb_bind_int16(guinsoodb_prepared_statement prepared_statement, idx_t param_idx, int16_t val) {
	return guinsoodb_bind_value(prepared_statement, param_idx, Value::SMALLINT(val));
}

guinsoodb_state guinsoodb_bind_int32(guinsoodb_prepared_statement prepared_statement, idx_t param_idx, int32_t val) {
	return guinsoodb_bind_value(prepared_statement, param_idx, Value::INTEGER(val));
}

guinsoodb_state guinsoodb_bind_int64(guinsoodb_prepared_statement prepared_statement, idx_t param_idx, int64_t val) {
	return guinsoodb_bind_value(prepared_statement, param_idx, Value::BIGINT(val));
}

guinsoodb_state guinsoodb_bind_uint8(guinsoodb_prepared_statement prepared_statement, idx_t param_idx, uint8_t val) {
	return guinsoodb_bind_value(prepared_statement, param_idx, Value::UTINYINT(val));
}

guinsoodb_state guinsoodb_bind_uint16(guinsoodb_prepared_statement prepared_statement, idx_t param_idx, uint16_t val) {
	return guinsoodb_bind_value(prepared_statement, param_idx, Value::USMALLINT(val));
}

guinsoodb_state guinsoodb_bind_uint32(guinsoodb_prepared_statement prepared_statement, idx_t param_idx, uint32_t val) {
	return guinsoodb_bind_value(prepared_statement, param_idx, Value::UINTEGER(val));
}

guinsoodb_state guinsoodb_bind_uint64(guinsoodb_prepared_statement prepared_statement, idx_t param_idx, uint64_t val) {
	return guinsoodb_bind_value(prepared_statement, param_idx, Value::UBIGINT(val));
}

guinsoodb_state guinsoodb_bind_float(guinsoodb_prepared_statement prepared_statement, idx_t param_idx, float val) {
	return guinsoodb_bind_value(prepared_statement, param_idx, Value(val));
}

guinsoodb_state guinsoodb_bind_double(guinsoodb_prepared_statement prepared_statement, idx_t param_idx, double val) {
	return guinsoodb_bind_value(prepared_statement, param_idx, Value(val));
}

guinsoodb_state guinsoodb_bind_varchar(guinsoodb_prepared_statement prepared_statement, idx_t param_idx, const char *val) {
	return guinsoodb_bind_value(prepared_statement, param_idx, Value(val));
}

guinsoodb_state guinsoodb_bind_varchar_length(guinsoodb_prepared_statement prepared_statement, idx_t param_idx, const char *val,
                                        idx_t length) {
	return guinsoodb_bind_value(prepared_statement, param_idx, Value(string(val, length)));
}

guinsoodb_state guinsoodb_bind_blob(guinsoodb_prepared_statement prepared_statement, idx_t param_idx, const void *data,
                              idx_t length) {
	return guinsoodb_bind_value(prepared_statement, param_idx, Value::BLOB((const_data_ptr_t)data, length));
}

guinsoodb_state guinsoodb_bind_null(guinsoodb_prepared_statement prepared_statement, idx_t param_idx) {
	return guinsoodb_bind_value(prepared_statement, param_idx, Value());
}

guinsoodb_state guinsoodb_execute_prepared(guinsoodb_prepared_statement prepared_statement, guinsoodb_result *out_result) {
	auto wrapper = (PreparedStatementWrapper *)prepared_statement;
	if (!wrapper || !wrapper->statement || !wrapper->statement->success) {
		return GuinsooDBError;
	}
	auto result = wrapper->statement->Execute(wrapper->values, false);
	D_ASSERT(result->type == QueryResultType::MATERIALIZED_RESULT);
	auto mat_res = (MaterializedQueryResult *)result.get();
	return guinsoodb_translate_result(mat_res, out_result);
}

void guinsoodb_destroy_prepare(guinsoodb_prepared_statement *prepared_statement) {
	if (!prepared_statement) {
		return;
	}
	auto wrapper = (PreparedStatementWrapper *)*prepared_statement;
	if (wrapper) {
		delete wrapper;
	}
	*prepared_statement = nullptr;
}

guinsoodb_type ConvertCPPTypeToC(LogicalType sql_type) {
	switch (sql_type.id()) {
	case LogicalTypeId::BOOLEAN:
		return GUINSOODB_TYPE_BOOLEAN;
	case LogicalTypeId::TINYINT:
		return GUINSOODB_TYPE_TINYINT;
	case LogicalTypeId::SMALLINT:
		return GUINSOODB_TYPE_SMALLINT;
	case LogicalTypeId::INTEGER:
		return GUINSOODB_TYPE_INTEGER;
	case LogicalTypeId::BIGINT:
		return GUINSOODB_TYPE_BIGINT;
	case LogicalTypeId::HUGEINT:
		return GUINSOODB_TYPE_HUGEINT;
	case LogicalTypeId::FLOAT:
		return GUINSOODB_TYPE_FLOAT;
	case LogicalTypeId::DOUBLE:
		return GUINSOODB_TYPE_DOUBLE;
	case LogicalTypeId::TIMESTAMP:
		return GUINSOODB_TYPE_TIMESTAMP;
	case LogicalTypeId::DATE:
		return GUINSOODB_TYPE_DATE;
	case LogicalTypeId::TIME:
		return GUINSOODB_TYPE_TIME;
	case LogicalTypeId::VARCHAR:
		return GUINSOODB_TYPE_VARCHAR;
	case LogicalTypeId::BLOB:
		return GUINSOODB_TYPE_BLOB;
	case LogicalTypeId::INTERVAL:
		return GUINSOODB_TYPE_INTERVAL;
	default:
		return GUINSOODB_TYPE_INVALID;
	}
}

idx_t GetCTypeSize(guinsoodb_type type) {
	switch (type) {
	case GUINSOODB_TYPE_BOOLEAN:
		return sizeof(bool);
	case GUINSOODB_TYPE_TINYINT:
		return sizeof(int8_t);
	case GUINSOODB_TYPE_SMALLINT:
		return sizeof(int16_t);
	case GUINSOODB_TYPE_INTEGER:
		return sizeof(int32_t);
	case GUINSOODB_TYPE_BIGINT:
		return sizeof(int64_t);
	case GUINSOODB_TYPE_HUGEINT:
		return sizeof(guinsoodb_hugeint);
	case GUINSOODB_TYPE_FLOAT:
		return sizeof(float);
	case GUINSOODB_TYPE_DOUBLE:
		return sizeof(double);
	case GUINSOODB_TYPE_DATE:
		return sizeof(guinsoodb_date);
	case GUINSOODB_TYPE_TIME:
		return sizeof(guinsoodb_time);
	case GUINSOODB_TYPE_TIMESTAMP:
		return sizeof(guinsoodb_timestamp);
	case GUINSOODB_TYPE_VARCHAR:
		return sizeof(const char *);
	case GUINSOODB_TYPE_BLOB:
		return sizeof(guinsoodb_blob);
	case GUINSOODB_TYPE_INTERVAL:
		return sizeof(guinsoodb_interval);
	default:
		// unsupported type
		D_ASSERT(0);
		return sizeof(const char *);
	}
}

template <class T>
T UnsafeFetch(guinsoodb_result *result, idx_t col, idx_t row) {
	D_ASSERT(row < result->row_count);
	return ((T *)result->columns[col].data)[row];
}

static Value GetCValue(guinsoodb_result *result, idx_t col, idx_t row) {
	if (col >= result->column_count) {
		return Value();
	}
	if (row >= result->row_count) {
		return Value();
	}
	if (result->columns[col].nullmask[row]) {
		return Value();
	}
	switch (result->columns[col].type) {
	case GUINSOODB_TYPE_BOOLEAN:
		return Value::BOOLEAN(UnsafeFetch<bool>(result, col, row));
	case GUINSOODB_TYPE_TINYINT:
		return Value::TINYINT(UnsafeFetch<int8_t>(result, col, row));
	case GUINSOODB_TYPE_SMALLINT:
		return Value::SMALLINT(UnsafeFetch<int16_t>(result, col, row));
	case GUINSOODB_TYPE_INTEGER:
		return Value::INTEGER(UnsafeFetch<int32_t>(result, col, row));
	case GUINSOODB_TYPE_BIGINT:
		return Value::BIGINT(UnsafeFetch<int64_t>(result, col, row));
	case GUINSOODB_TYPE_FLOAT:
		return Value(UnsafeFetch<float>(result, col, row));
	case GUINSOODB_TYPE_DOUBLE:
		return Value(UnsafeFetch<double>(result, col, row));
	case GUINSOODB_TYPE_DATE: {
		auto date = UnsafeFetch<guinsoodb_date>(result, col, row);
		return Value::DATE(date.year, date.month, date.day);
	}
	case GUINSOODB_TYPE_TIME: {
		auto time = UnsafeFetch<guinsoodb_time>(result, col, row);
		return Value::TIME(time.hour, time.min, time.sec, time.micros);
	}
	case GUINSOODB_TYPE_TIMESTAMP: {
		auto timestamp = UnsafeFetch<guinsoodb_timestamp>(result, col, row);
		return Value::TIMESTAMP(timestamp.date.year, timestamp.date.month, timestamp.date.day, timestamp.time.hour,
		                        timestamp.time.min, timestamp.time.sec, timestamp.time.micros);
	}
	case GUINSOODB_TYPE_HUGEINT: {
		hugeint_t val;
		auto hugeint = UnsafeFetch<guinsoodb_hugeint>(result, col, row);
		val.lower = hugeint.lower;
		val.upper = hugeint.upper;
		return Value::HUGEINT(val);
	}
	case GUINSOODB_TYPE_INTERVAL: {
		interval_t val;
		auto interval = UnsafeFetch<guinsoodb_interval>(result, col, row);
		val.days = interval.days;
		val.months = interval.months;
		val.micros = interval.micros;
		return Value::INTERVAL(val);
	}
	case GUINSOODB_TYPE_VARCHAR:
		return Value(string(UnsafeFetch<const char *>(result, col, row)));
	case GUINSOODB_TYPE_BLOB: {
		auto blob = UnsafeFetch<guinsoodb_blob>(result, col, row);
		return Value::BLOB((const_data_ptr_t)blob.data, blob.size);
	}
	default:
		// invalid type for C to C++ conversion
		D_ASSERT(0);
		return Value();
	}
}

const char *guinsoodb_column_name(guinsoodb_result *result, idx_t col) {
	if (!result || col >= result->column_count) {
		return nullptr;
	}
	return result->columns[col].name;
}

bool guinsoodb_value_boolean(guinsoodb_result *result, idx_t col, idx_t row) {
	Value val = GetCValue(result, col, row);
	if (val.is_null) {
		return false;
	} else {
		return val.GetValue<bool>();
	}
}

int8_t guinsoodb_value_int8(guinsoodb_result *result, idx_t col, idx_t row) {
	Value val = GetCValue(result, col, row);
	if (val.is_null) {
		return 0;
	} else {
		return val.GetValue<int8_t>();
	}
}

int16_t guinsoodb_value_int16(guinsoodb_result *result, idx_t col, idx_t row) {
	Value val = GetCValue(result, col, row);
	if (val.is_null) {
		return 0;
	} else {
		return val.GetValue<int16_t>();
	}
}

int32_t guinsoodb_value_int32(guinsoodb_result *result, idx_t col, idx_t row) {
	Value val = GetCValue(result, col, row);
	if (val.is_null) {
		return 0;
	} else {
		return val.GetValue<int32_t>();
	}
}

int64_t guinsoodb_value_int64(guinsoodb_result *result, idx_t col, idx_t row) {
	Value val = GetCValue(result, col, row);
	if (val.is_null) {
		return 0;
	} else {
		return val.GetValue<int64_t>();
	}
}

uint8_t guinsoodb_value_uint8(guinsoodb_result *result, idx_t col, idx_t row) {
	Value val = GetCValue(result, col, row);
	if (val.is_null) {
		return 0;
	} else {
		return val.GetValue<uint8_t>();
	}
}

uint16_t guinsoodb_value_uint16(guinsoodb_result *result, idx_t col, idx_t row) {
	Value val = GetCValue(result, col, row);
	if (val.is_null) {
		return 0;
	} else {
		return val.GetValue<uint16_t>();
	}
}

uint32_t guinsoodb_value_uint32(guinsoodb_result *result, idx_t col, idx_t row) {
	Value val = GetCValue(result, col, row);
	if (val.is_null) {
		return 0;
	} else {
		return val.GetValue<uint32_t>();
	}
}

uint64_t guinsoodb_value_uint64(guinsoodb_result *result, idx_t col, idx_t row) {
	Value val = GetCValue(result, col, row);
	if (val.is_null) {
		return 0;
	} else {
		return val.GetValue<uint64_t>();
	}
}

float guinsoodb_value_float(guinsoodb_result *result, idx_t col, idx_t row) {
	Value val = GetCValue(result, col, row);
	if (val.is_null) {
		return 0.0;
	} else {
		return val.GetValue<float>();
	}
}

double guinsoodb_value_double(guinsoodb_result *result, idx_t col, idx_t row) {
	Value val = GetCValue(result, col, row);
	if (val.is_null) {
		return 0.0;
	} else {
		return val.GetValue<double>();
	}
}

char *guinsoodb_value_varchar(guinsoodb_result *result, idx_t col, idx_t row) {
	Value val = GetCValue(result, col, row);
	return strdup(val.ToString().c_str());
}

guinsoodb_blob guinsoodb_value_blob(guinsoodb_result *result, idx_t col, idx_t row) {
	guinsoodb_blob blob;
	Value val = GetCValue(result, col, row).CastAs(LogicalType::BLOB);
	if (val.is_null) {
		blob.data = nullptr;
		blob.size = 0;
	} else {
		blob.data = malloc(val.str_value.size());
		memcpy((void *)blob.data, val.str_value.c_str(), val.str_value.size());
		blob.size = val.str_value.size();
	}
	return blob;
}

guinsoodb_state guinsoodb_appender_create(guinsoodb_connection connection, const char *schema, const char *table,
                                    guinsoodb_appender *out_appender) {
	Connection *conn = (Connection *)connection;

	if (!connection || !table || !out_appender) {
		return GuinsooDBError;
	}
	if (schema == nullptr) {

		schema = DEFAULT_SCHEMA;
	}
	try {
		auto *appender = new Appender(*conn, schema, table);
		*out_appender = appender;
	} catch (...) {
		return GuinsooDBError;
	}
	return GuinsooDBSuccess;
}

guinsoodb_state guinsoodb_appender_destroy(guinsoodb_appender *appender) {
	if (!appender || !*appender) {
		return GuinsooDBError;
	}
	auto *appender_instance = *((Appender **)appender);
	delete appender_instance;
	*appender = nullptr;
	return GuinsooDBSuccess;
}

#define APPENDER_CALL(FUN)                                                                                             \
	if (!appender) {                                                                                                   \
		return GuinsooDBError;                                                                                            \
	}                                                                                                                  \
	auto *appender_instance = (Appender *)appender;                                                                    \
	try {                                                                                                              \
		appender_instance->FUN();                                                                                      \
	} catch (...) {                                                                                                    \
		return GuinsooDBError;                                                                                            \
	}                                                                                                                  \
	return GuinsooDBSuccess;

#define APPENDER_CALL_PARAM(FUN, PARAM)                                                                                \
	if (!appender) {                                                                                                   \
		return GuinsooDBError;                                                                                            \
	}                                                                                                                  \
	auto *appender_instance = (Appender *)appender;                                                                    \
	try {                                                                                                              \
		appender_instance->FUN(PARAM);                                                                                 \
	} catch (...) {                                                                                                    \
		return GuinsooDBError;                                                                                            \
	}                                                                                                                  \
	return GuinsooDBSuccess;

guinsoodb_state guinsoodb_appender_begin_row(guinsoodb_appender appender) {
	APPENDER_CALL(BeginRow);
}

guinsoodb_state guinsoodb_appender_end_row(guinsoodb_appender appender) {
	APPENDER_CALL(EndRow);
}

guinsoodb_state guinsoodb_append_bool(guinsoodb_appender appender, bool value) {
	APPENDER_CALL_PARAM(Append<bool>, value);
}

guinsoodb_state guinsoodb_append_int8(guinsoodb_appender appender, int8_t value) {
	APPENDER_CALL_PARAM(Append<int8_t>, value);
}

guinsoodb_state guinsoodb_append_int16(guinsoodb_appender appender, int16_t value) {
	APPENDER_CALL_PARAM(Append<int16_t>, value);
}

guinsoodb_state guinsoodb_append_int32(guinsoodb_appender appender, int32_t value) {
	APPENDER_CALL_PARAM(Append<int32_t>, value);
}

guinsoodb_state guinsoodb_append_int64(guinsoodb_appender appender, int64_t value) {
	APPENDER_CALL_PARAM(Append<int64_t>, value);
}

guinsoodb_state guinsoodb_append_uint8(guinsoodb_appender appender, uint8_t value) {
	APPENDER_CALL_PARAM(Append<uint8_t>, value);
}

guinsoodb_state guinsoodb_append_uint16(guinsoodb_appender appender, uint16_t value) {
	APPENDER_CALL_PARAM(Append<uint16_t>, value);
}

guinsoodb_state guinsoodb_append_uint32(guinsoodb_appender appender, uint32_t value) {
	APPENDER_CALL_PARAM(Append<uint32_t>, value);
}

guinsoodb_state guinsoodb_append_uint64(guinsoodb_appender appender, uint64_t value) {
	APPENDER_CALL_PARAM(Append<uint64_t>, value);
}

guinsoodb_state guinsoodb_append_float(guinsoodb_appender appender, float value) {
	APPENDER_CALL_PARAM(Append<float>, value);
}

guinsoodb_state guinsoodb_append_double(guinsoodb_appender appender, double value) {
	APPENDER_CALL_PARAM(Append<double>, value);
}

guinsoodb_state guinsoodb_append_null(guinsoodb_appender appender) {
	APPENDER_CALL_PARAM(Append<std::nullptr_t>, nullptr);
}

guinsoodb_state guinsoodb_append_varchar(guinsoodb_appender appender, const char *val) {
	auto string_val = Value(val);
	APPENDER_CALL_PARAM(Append<Value>, string_val);
}

guinsoodb_state guinsoodb_append_varchar_length(guinsoodb_appender appender, const char *val, idx_t length) {
	auto string_val = Value(string(val, length)); // TODO this copies orr
	APPENDER_CALL_PARAM(Append<Value>, string_val);
}
guinsoodb_state guinsoodb_append_blob(guinsoodb_appender appender, const void *data, idx_t length) {
	auto blob_val = Value::BLOB((const_data_ptr_t)data, length);
	APPENDER_CALL_PARAM(Append<Value>, blob_val);
}

guinsoodb_state guinsoodb_appender_flush(guinsoodb_appender appender) {
	APPENDER_CALL(Flush);
}

guinsoodb_state guinsoodb_appender_close(guinsoodb_appender appender) {
	APPENDER_CALL(Close);
}

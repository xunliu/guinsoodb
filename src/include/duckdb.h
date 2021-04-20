//===----------------------------------------------------------------------===//
//
//                         GuinsooDB
//
// guinsoodb.h
//
// Author: Mark Raasveldt
//
//===----------------------------------------------------------------------===//

#pragma once

#ifdef _WIN32
#ifdef DUCKDB_BUILD_LIBRARY
#define DUCKDB_API __declspec(dllexport)
#else
#define DUCKDB_API __declspec(dllimport)
#endif
#else
#define DUCKDB_API
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t idx_t;

typedef enum DUCKDB_TYPE {
	DUCKDB_TYPE_INVALID = 0,
	// bool
	DUCKDB_TYPE_BOOLEAN,
	// int8_t
	DUCKDB_TYPE_TINYINT,
	// int16_t
	DUCKDB_TYPE_SMALLINT,
	// int32_t
	DUCKDB_TYPE_INTEGER,
	// int64_t
	DUCKDB_TYPE_BIGINT,
	// float
	DUCKDB_TYPE_FLOAT,
	// double
	DUCKDB_TYPE_DOUBLE,
	// guinsoodb_timestamp
	DUCKDB_TYPE_TIMESTAMP,
	// guinsoodb_date
	DUCKDB_TYPE_DATE,
	// guinsoodb_time
	DUCKDB_TYPE_TIME,
	// guinsoodb_interval
	DUCKDB_TYPE_INTERVAL,
	// guinsoodb_hugeint
	DUCKDB_TYPE_HUGEINT,
	// const char*
	DUCKDB_TYPE_VARCHAR,
	// guinsoodb_blob
	DUCKDB_TYPE_BLOB
} guinsoodb_type;

typedef struct {
	int32_t year;
	int8_t month;
	int8_t day;
} guinsoodb_date;

typedef struct {
	int8_t hour;
	int8_t min;
	int8_t sec;
	int16_t micros;
} guinsoodb_time;

typedef struct {
	guinsoodb_date date;
	guinsoodb_time time;
} guinsoodb_timestamp;

typedef struct {
	int32_t months;
	int32_t days;
	int64_t micros;
} guinsoodb_interval;

typedef struct {
	uint64_t lower;
	int64_t upper;
} guinsoodb_hugeint;

typedef struct {
	void *data;
	idx_t size;
} guinsoodb_blob;

typedef struct {
	void *data;
	bool *nullmask;
	guinsoodb_type type;
	char *name;
} guinsoodb_column;

typedef struct {
	idx_t column_count;
	idx_t row_count;
	guinsoodb_column *columns;
	char *error_message;
} guinsoodb_result;

// typedef struct {
// 	void *data;
// 	bool *nullmask;
// } guinsoodb_column_data;

// typedef struct {
// 	int column_count;
// 	int count;
// 	guinsoodb_column_data *columns;
// } guinsoodb_chunk;

typedef void *guinsoodb_database;
typedef void *guinsoodb_connection;
typedef void *guinsoodb_prepared_statement;
typedef void *guinsoodb_appender;

typedef enum { GuinsooDBSuccess = 0, GuinsooDBError = 1 } guinsoodb_state;

//! Opens a database file at the given path (nullptr for in-memory). Returns GuinsooDBSuccess on success, or GuinsooDBError on
//! failure. [OUT: database]
DUCKDB_API guinsoodb_state guinsoodb_open(const char *path, guinsoodb_database *out_database);
//! Closes the database.
DUCKDB_API void guinsoodb_close(guinsoodb_database *database);

//! Creates a connection to the specified database. [OUT: connection]
DUCKDB_API guinsoodb_state guinsoodb_connect(guinsoodb_database database, guinsoodb_connection *out_connection);
//! Closes the specified connection handle
DUCKDB_API void guinsoodb_disconnect(guinsoodb_connection *connection);

//! Executes the specified SQL query in the specified connection handle. [OUT: result descriptor]
DUCKDB_API guinsoodb_state guinsoodb_query(guinsoodb_connection connection, const char *query, guinsoodb_result *out_result);
//! Destroys the specified result
DUCKDB_API void guinsoodb_destroy_result(guinsoodb_result *result);

//! Returns the column name of the specified column. The result does not need to be freed;
//! the column names will automatically be destroyed when the result is destroyed.
DUCKDB_API const char *guinsoodb_column_name(guinsoodb_result *result, idx_t col);

// SAFE fetch functions
// These functions will perform conversions if necessary. On failure (e.g. if conversion cannot be performed) a special
// value is returned.

//! Converts the specified value to a bool. Returns false on failure or NULL.
DUCKDB_API bool guinsoodb_value_boolean(guinsoodb_result *result, idx_t col, idx_t row);
//! Converts the specified value to an int8_t. Returns 0 on failure or NULL.
DUCKDB_API int8_t guinsoodb_value_int8(guinsoodb_result *result, idx_t col, idx_t row);
//! Converts the specified value to an int16_t. Returns 0 on failure or NULL.
DUCKDB_API int16_t guinsoodb_value_int16(guinsoodb_result *result, idx_t col, idx_t row);
//! Converts the specified value to an int64_t. Returns 0 on failure or NULL.
DUCKDB_API int32_t guinsoodb_value_int32(guinsoodb_result *result, idx_t col, idx_t row);
//! Converts the specified value to an int64_t. Returns 0 on failure or NULL.
DUCKDB_API int64_t guinsoodb_value_int64(guinsoodb_result *result, idx_t col, idx_t row);
//! Converts the specified value to an uint8_t. Returns 0 on failure or NULL.
DUCKDB_API uint8_t guinsoodb_value_uint8(guinsoodb_result *result, idx_t col, idx_t row);
//! Converts the specified value to an uint16_t. Returns 0 on failure or NULL.
DUCKDB_API uint16_t guinsoodb_value_uint16(guinsoodb_result *result, idx_t col, idx_t row);
//! Converts the specified value to an uint64_t. Returns 0 on failure or NULL.
DUCKDB_API uint32_t guinsoodb_value_uint32(guinsoodb_result *result, idx_t col, idx_t row);
//! Converts the specified value to an uint64_t. Returns 0 on failure or NULL.
DUCKDB_API uint64_t guinsoodb_value_uint64(guinsoodb_result *result, idx_t col, idx_t row);
//! Converts the specified value to a float. Returns 0.0 on failure or NULL.
DUCKDB_API float guinsoodb_value_float(guinsoodb_result *result, idx_t col, idx_t row);
//! Converts the specified value to a double. Returns 0.0 on failure or NULL.
DUCKDB_API double guinsoodb_value_double(guinsoodb_result *result, idx_t col, idx_t row);
//! Converts the specified value to a string. Returns nullptr on failure or NULL. The result must be freed with free.
DUCKDB_API char *guinsoodb_value_varchar(guinsoodb_result *result, idx_t col, idx_t row);
//! Fetches a blob from a result set column. Returns a blob with blob.data set to nullptr on failure or NULL. The
//! resulting "blob.data" must be freed with free.
DUCKDB_API guinsoodb_blob guinsoodb_value_blob(guinsoodb_result *result, idx_t col, idx_t row);

// Prepared Statements

//! prepares the specified SQL query in the specified connection handle. [OUT: prepared statement descriptor]
DUCKDB_API guinsoodb_state guinsoodb_prepare(guinsoodb_connection connection, const char *query,
                                       guinsoodb_prepared_statement *out_prepared_statement);

DUCKDB_API guinsoodb_state guinsoodb_nparams(guinsoodb_prepared_statement prepared_statement, idx_t *nparams_out);

//! binds parameters to prepared statement
DUCKDB_API guinsoodb_state guinsoodb_bind_boolean(guinsoodb_prepared_statement prepared_statement, idx_t param_idx, bool val);
DUCKDB_API guinsoodb_state guinsoodb_bind_int8(guinsoodb_prepared_statement prepared_statement, idx_t param_idx, int8_t val);
DUCKDB_API guinsoodb_state guinsoodb_bind_int16(guinsoodb_prepared_statement prepared_statement, idx_t param_idx, int16_t val);
DUCKDB_API guinsoodb_state guinsoodb_bind_int32(guinsoodb_prepared_statement prepared_statement, idx_t param_idx, int32_t val);
DUCKDB_API guinsoodb_state guinsoodb_bind_int64(guinsoodb_prepared_statement prepared_statement, idx_t param_idx, int64_t val);
DUCKDB_API guinsoodb_state guinsoodb_bind_uint8(guinsoodb_prepared_statement prepared_statement, idx_t param_idx, int8_t val);
DUCKDB_API guinsoodb_state guinsoodb_bind_uint16(guinsoodb_prepared_statement prepared_statement, idx_t param_idx, int16_t val);
DUCKDB_API guinsoodb_state guinsoodb_bind_uint32(guinsoodb_prepared_statement prepared_statement, idx_t param_idx, uint32_t val);
DUCKDB_API guinsoodb_state guinsoodb_bind_uint64(guinsoodb_prepared_statement prepared_statement, idx_t param_idx, uint64_t val);
DUCKDB_API guinsoodb_state guinsoodb_bind_float(guinsoodb_prepared_statement prepared_statement, idx_t param_idx, float val);
DUCKDB_API guinsoodb_state guinsoodb_bind_double(guinsoodb_prepared_statement prepared_statement, idx_t param_idx, double val);
DUCKDB_API guinsoodb_state guinsoodb_bind_varchar(guinsoodb_prepared_statement prepared_statement, idx_t param_idx,
                                            const char *val);
DUCKDB_API guinsoodb_state guinsoodb_bind_varchar_length(guinsoodb_prepared_statement prepared_statement, idx_t param_idx,
                                                   const char *val, idx_t length);
DUCKDB_API guinsoodb_state guinsoodb_bind_blob(guinsoodb_prepared_statement prepared_statement, idx_t param_idx,
                                         const void *data, idx_t length);
DUCKDB_API guinsoodb_state guinsoodb_bind_null(guinsoodb_prepared_statement prepared_statement, idx_t param_idx);

//! Executes the prepared statements with currently bound parameters
DUCKDB_API guinsoodb_state guinsoodb_execute_prepared(guinsoodb_prepared_statement prepared_statement,
                                                guinsoodb_result *out_result);

//! Destroys the specified prepared statement descriptor
DUCKDB_API void guinsoodb_destroy_prepare(guinsoodb_prepared_statement *prepared_statement);

DUCKDB_API guinsoodb_state guinsoodb_appender_create(guinsoodb_connection connection, const char *schema, const char *table,
                                               guinsoodb_appender *out_appender);

DUCKDB_API guinsoodb_state guinsoodb_appender_begin_row(guinsoodb_appender appender);
DUCKDB_API guinsoodb_state guinsoodb_appender_end_row(guinsoodb_appender appender);

DUCKDB_API guinsoodb_state guinsoodb_append_bool(guinsoodb_appender appender, bool value);

DUCKDB_API guinsoodb_state guinsoodb_append_int8(guinsoodb_appender appender, int8_t value);
DUCKDB_API guinsoodb_state guinsoodb_append_int16(guinsoodb_appender appender, int16_t value);
DUCKDB_API guinsoodb_state guinsoodb_append_int32(guinsoodb_appender appender, int32_t value);
DUCKDB_API guinsoodb_state guinsoodb_append_int64(guinsoodb_appender appender, int64_t value);

DUCKDB_API guinsoodb_state guinsoodb_append_uint8(guinsoodb_appender appender, uint8_t value);
DUCKDB_API guinsoodb_state guinsoodb_append_uint16(guinsoodb_appender appender, uint16_t value);
DUCKDB_API guinsoodb_state guinsoodb_append_uint32(guinsoodb_appender appender, uint32_t value);
DUCKDB_API guinsoodb_state guinsoodb_append_uint64(guinsoodb_appender appender, uint64_t value);

DUCKDB_API guinsoodb_state guinsoodb_append_float(guinsoodb_appender appender, float value);
DUCKDB_API guinsoodb_state guinsoodb_append_double(guinsoodb_appender appender, double value);

DUCKDB_API guinsoodb_state guinsoodb_append_varchar(guinsoodb_appender appender, const char *val);
DUCKDB_API guinsoodb_state guinsoodb_append_varchar_length(guinsoodb_appender appender, const char *val, idx_t length);
DUCKDB_API guinsoodb_state guinsoodb_append_blob(guinsoodb_appender appender, const void *data, idx_t length);
DUCKDB_API guinsoodb_state guinsoodb_append_null(guinsoodb_appender appender);

DUCKDB_API guinsoodb_state guinsoodb_appender_flush(guinsoodb_appender appender);
DUCKDB_API guinsoodb_state guinsoodb_appender_close(guinsoodb_appender appender);

DUCKDB_API guinsoodb_state guinsoodb_appender_destroy(guinsoodb_appender *appender);

#ifdef __cplusplus
}
#endif

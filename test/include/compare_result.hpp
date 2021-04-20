#pragma once

#include "guinsoodb/common/string_util.hpp"
#include "guinsoodb.hpp"

namespace guinsoodb {

bool CHECK_COLUMN(QueryResult &result, size_t column_number, vector<guinsoodb::Value> values);
bool CHECK_COLUMN(unique_ptr<guinsoodb::QueryResult> &result, size_t column_number, vector<guinsoodb::Value> values);
bool CHECK_COLUMN(unique_ptr<guinsoodb::MaterializedQueryResult> &result, size_t column_number,
                  vector<guinsoodb::Value> values);

string compare_csv(guinsoodb::QueryResult &result, string csv, bool header = false);

bool parse_datachunk(string csv, DataChunk &result, vector<LogicalType> sql_types, bool has_header);

//! Compares the result of a pipe-delimited CSV with the given DataChunk
//! Returns true if they are equal, and stores an error_message otherwise
bool compare_result(string csv, ChunkCollection &collection, vector<LogicalType> sql_types, bool has_header,
                    string &error_message);

} // namespace guinsoodb

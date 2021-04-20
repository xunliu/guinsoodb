//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/main/materialized_query_result.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/types/chunk_collection.hpp"
#include "guinsoodb/common/winapi.hpp"
#include "guinsoodb/main/query_result.hpp"

namespace guinsoodb {

class MaterializedQueryResult : public QueryResult {
public:
	//! Creates an empty successful query result
	GUINSOODB_API explicit MaterializedQueryResult(StatementType statement_type);
	//! Creates a successful query result with the specified names and types
	GUINSOODB_API MaterializedQueryResult(StatementType statement_type, vector<LogicalType> types, vector<string> names);
	//! Creates an unsuccessful query result with error condition
	GUINSOODB_API explicit MaterializedQueryResult(string error);

	ChunkCollection collection;

public:
	//! Fetches a DataChunk from the query result.
	//! This will consume the result (i.e. the chunks are taken directly from the ChunkCollection).
	GUINSOODB_API unique_ptr<DataChunk> Fetch() override;
	GUINSOODB_API unique_ptr<DataChunk> FetchRaw() override;
	//! Converts the QueryResult to a string
	GUINSOODB_API string ToString() override;

	//! Gets the (index) value of the (column index) column
	GUINSOODB_API Value GetValue(idx_t column, idx_t index);

	template <class T>
	T GetValue(idx_t column, idx_t index) {
		auto value = GetValue(column, index);
		return (T)value.GetValue<int64_t>();
	}
};

} // namespace guinsoodb
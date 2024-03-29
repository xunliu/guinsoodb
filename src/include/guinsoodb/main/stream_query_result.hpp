//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/main/stream_query_result.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/winapi.hpp"
#include "guinsoodb/main/query_result.hpp"

namespace guinsoodb {

class ClientContext;
class MaterializedQueryResult;
class PreparedStatementData;

class StreamQueryResult : public QueryResult {
public:
	//! Create a successful StreamQueryResult. StreamQueryResults should always be successful initially (it makes no
	//! sense to stream an error).
	GUINSOODB_API StreamQueryResult(StatementType statement_type, shared_ptr<ClientContext> context,
	                             vector<LogicalType> types, vector<string> names,
	                             shared_ptr<PreparedStatementData> prepared = nullptr);
	GUINSOODB_API ~StreamQueryResult() override;

	//! Whether or not the StreamQueryResult is still open
	bool is_open;

public:
	//! Fetches a DataChunk from the query result.
	GUINSOODB_API unique_ptr<DataChunk> FetchRaw() override;
	//! Converts the QueryResult to a string
	GUINSOODB_API string ToString() override;
	//! Materializes the query result and turns it into a materialized query result
	GUINSOODB_API unique_ptr<MaterializedQueryResult> Materialize();

	//! Closes the StreamQueryResult
	GUINSOODB_API void Close();

private:
	//! The client context this StreamQueryResult belongs to
	shared_ptr<ClientContext> context;
	//! The prepared statement data this StreamQueryResult was created with (if any)
	shared_ptr<PreparedStatementData> prepared;
};

} // namespace guinsoodb
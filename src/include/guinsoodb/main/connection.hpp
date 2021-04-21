//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/main/connection.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/enums/profiler_format.hpp"
#include "guinsoodb/common/serializer/buffered_file_writer.hpp"
#include "guinsoodb/common/winapi.hpp"
#include "guinsoodb/function/udf_function.hpp"
#include "guinsoodb/main/materialized_query_result.hpp"
#include "guinsoodb/main/prepared_statement.hpp"
#include "guinsoodb/main/query_result.hpp"
#include "guinsoodb/main/relation.hpp"
#include "guinsoodb/main/stream_query_result.hpp"
#include "guinsoodb/main/table_description.hpp"
#include "guinsoodb/parser/sql_statement.hpp"

namespace guinsoodb {

class ClientContext;
class DatabaseInstance;
class GuinsooDB;

typedef void (*warning_callback)(std::string);

//! A connection to a database. This represents a (client) connection that can
//! be used to query the database.
class Connection {
public:
	GUINSOODB_API explicit Connection(GuinsooDB &database);
	GUINSOODB_API explicit Connection(DatabaseInstance &database);

	shared_ptr<ClientContext> context;
	warning_callback warning_cb;

public:
	//! Returns query profiling information for the current query
	GUINSOODB_API string GetProfilingInformation(ProfilerPrintFormat format = ProfilerPrintFormat::QUERY_TREE);

	//! Interrupt execution of the current query
	GUINSOODB_API void Interrupt();

	//! Enable query profiling
	GUINSOODB_API void EnableProfiling();
	//! Disable query profiling
	GUINSOODB_API void DisableProfiling();

	GUINSOODB_API void SetWarningCallback(warning_callback);

	//! Enable aggressive verification/testing of queries, should only be used in testing
	GUINSOODB_API void EnableQueryVerification();
	GUINSOODB_API void DisableQueryVerification();
	//! Force parallel execution, even for smaller tables. Should only be used in testing.
	GUINSOODB_API void ForceParallelism();

	//! Issues a query to the database and returns a QueryResult. This result can be either a StreamQueryResult or a
	//! MaterializedQueryResult. The result can be stepped through with calls to Fetch(). Note that there can only be
	//! one active StreamQueryResult per Connection object. Calling SendQuery() will invalidate any previously existing
	//! StreamQueryResult.
	GUINSOODB_API unique_ptr<QueryResult> SendQuery(const string &query);
	//! Issues a query to the database and materializes the result (if necessary). Always returns a
	//! MaterializedQueryResult.
	GUINSOODB_API unique_ptr<MaterializedQueryResult> Query(const string &query);
	//! Issues a query to the database and materializes the result (if necessary). Always returns a
	//! MaterializedQueryResult.
	GUINSOODB_API unique_ptr<MaterializedQueryResult> Query(unique_ptr<SQLStatement> statement);
	// prepared statements
	template <typename... Args>
	unique_ptr<QueryResult> Query(const string &query, Args... args) {
		vector<Value> values;
		return QueryParamsRecursive(query, values, args...);
	}

	//! Prepare the specified query, returning a prepared statement object
	GUINSOODB_API unique_ptr<PreparedStatement> Prepare(const string &query);
	//! Prepare the specified statement, returning a prepared statement object
	GUINSOODB_API unique_ptr<PreparedStatement> Prepare(unique_ptr<SQLStatement> statement);

	//! Get the table info of a specific table (in the default schema), or nullptr if it cannot be found
	GUINSOODB_API unique_ptr<TableDescription> TableInfo(const string &table_name);
	//! Get the table info of a specific table, or nullptr if it cannot be found
	GUINSOODB_API unique_ptr<TableDescription> TableInfo(const string &schema_name, const string &table_name);

	//! Extract a set of SQL statements from a specific query
	GUINSOODB_API vector<unique_ptr<SQLStatement>> ExtractStatements(const string &query);

	//! Appends a DataChunk to the specified table
	GUINSOODB_API void Append(TableDescription &description, DataChunk &chunk);

	//! Returns a relation that produces a table from this connection
	GUINSOODB_API shared_ptr<Relation> Table(const string &tname);
	GUINSOODB_API shared_ptr<Relation> Table(const string &schema_name, const string &table_name);
	//! Returns a relation that produces a view from this connection
	GUINSOODB_API shared_ptr<Relation> View(const string &tname);
	GUINSOODB_API shared_ptr<Relation> View(const string &schema_name, const string &table_name);
	//! Returns a relation that calls a specified table function
	GUINSOODB_API shared_ptr<Relation> TableFunction(const string &tname);
	GUINSOODB_API shared_ptr<Relation> TableFunction(const string &tname, const vector<Value> &values);
	//! Returns a relation that produces values
	GUINSOODB_API shared_ptr<Relation> Values(const vector<vector<Value>> &values);
	GUINSOODB_API shared_ptr<Relation> Values(const vector<vector<Value>> &values, const vector<string> &column_names,
	                                       const string &alias = "values");
	GUINSOODB_API shared_ptr<Relation> Values(const string &values);
	GUINSOODB_API shared_ptr<Relation> Values(const string &values, const vector<string> &column_names,
	                                       const string &alias = "values");
	//! Reads CSV file
	GUINSOODB_API shared_ptr<Relation> ReadCSV(const string &csv_file);
	GUINSOODB_API shared_ptr<Relation> ReadCSV(const string &csv_file, const vector<string> &columns);

	GUINSOODB_API void BeginTransaction();
	GUINSOODB_API void Commit();
	GUINSOODB_API void Rollback();
	GUINSOODB_API void SetAutoCommit(bool auto_commit);
	GUINSOODB_API bool IsAutoCommit();

	template <typename TR, typename... Args>
	void CreateScalarFunction(const string &name, TR (*udf_func)(Args...)) {
		scalar_function_t function = UDFWrapper::CreateScalarFunction<TR, Args...>(name, udf_func);
		UDFWrapper::RegisterFunction<TR, Args...>(name, function, *context);
	}

	template <typename TR, typename... Args>
	void CreateScalarFunction(const string &name, vector<LogicalType> args, LogicalType ret_type,
	                          TR (*udf_func)(Args...)) {
		scalar_function_t function =
		    UDFWrapper::CreateScalarFunction<TR, Args...>(name, args, move(ret_type), udf_func);
		UDFWrapper::RegisterFunction(name, args, ret_type, function, *context);
	}

	template <typename TR, typename... Args>
	void CreateVectorizedFunction(const string &name, scalar_function_t udf_func,
	                              LogicalType varargs = LogicalType::INVALID) {
		UDFWrapper::RegisterFunction<TR, Args...>(name, udf_func, *context, move(varargs));
	}

	GUINSOODB_API void CreateVectorizedFunction(const string &name, vector<LogicalType> args, LogicalType ret_type,
	                                         scalar_function_t udf_func, LogicalType varargs = LogicalType::INVALID) {
		UDFWrapper::RegisterFunction(name, move(args), move(ret_type), udf_func, *context, move(varargs));
	}

	//------------------------------------- Aggreate Functions ----------------------------------------//
	template <typename UDF_OP, typename STATE, typename TR, typename TA>
	void CreateAggregateFunction(const string &name) {
		AggregateFunction function = UDFWrapper::CreateAggregateFunction<UDF_OP, STATE, TR, TA>(name);
		UDFWrapper::RegisterAggrFunction(function, *context);
	}

	template <typename UDF_OP, typename STATE, typename TR, typename TA, typename TB>
	void CreateAggregateFunction(const string &name) {
		AggregateFunction function = UDFWrapper::CreateAggregateFunction<UDF_OP, STATE, TR, TA, TB>(name);
		UDFWrapper::RegisterAggrFunction(function, *context);
	}

	template <typename UDF_OP, typename STATE, typename TR, typename TA>
	void CreateAggregateFunction(const string &name, LogicalType ret_type, LogicalType input_typeA) {
		AggregateFunction function =
		    UDFWrapper::CreateAggregateFunction<UDF_OP, STATE, TR, TA>(name, ret_type, input_typeA);
		UDFWrapper::RegisterAggrFunction(function, *context);
	}

	template <typename UDF_OP, typename STATE, typename TR, typename TA, typename TB>
	void CreateAggregateFunction(const string &name, LogicalType ret_type, LogicalType input_typeA,
	                             LogicalType input_typeB) {
		AggregateFunction function =
		    UDFWrapper::CreateAggregateFunction<UDF_OP, STATE, TR, TA, TB>(name, ret_type, input_typeA, input_typeB);
		UDFWrapper::RegisterAggrFunction(function, *context);
	}

	GUINSOODB_API void CreateAggregateFunction(const string &name, vector<LogicalType> arguments, LogicalType return_type,
	                                        aggregate_size_t state_size, aggregate_initialize_t initialize,
	                                        aggregate_update_t update, aggregate_combine_t combine,
	                                        aggregate_finalize_t finalize,
	                                        aggregate_simple_update_t simple_update = nullptr,
	                                        bind_aggregate_function_t bind = nullptr,
	                                        aggregate_destructor_t destructor = nullptr) {
		AggregateFunction function =
		    UDFWrapper::CreateAggregateFunction(name, arguments, return_type, state_size, initialize, update, combine,
		                                        finalize, simple_update, bind, destructor);
		UDFWrapper::RegisterAggrFunction(function, *context);
	}

private:
	unique_ptr<QueryResult> QueryParamsRecursive(const string &query, vector<Value> &values);

	template <typename T, typename... Args>
	unique_ptr<QueryResult> QueryParamsRecursive(const string &query, vector<Value> &values, T value, Args... args) {
		values.push_back(Value::CreateValue<T>(value));
		return QueryParamsRecursive(query, values, args...);
	}
};

} // namespace guinsoodb
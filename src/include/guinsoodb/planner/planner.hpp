//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/planner.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/sql_statement.hpp"
#include "guinsoodb/planner/binder.hpp"
#include "guinsoodb/planner/logical_operator.hpp"

namespace guinsoodb {
class ClientContext;
class PreparedStatementData;

//! The planner creates a logical query plan from the parsed SQL statements
//! using the Binder and LogicalPlanGenerator.
class Planner {
public:
	explicit Planner(ClientContext &context);

	void CreatePlan(unique_ptr<SQLStatement> statement);

	unique_ptr<LogicalOperator> plan;
	vector<string> names;
	vector<LogicalType> types;
	unordered_map<idx_t, vector<unique_ptr<Value>>> value_map;

	shared_ptr<Binder> binder;
	ClientContext &context;

	bool read_only;
	bool requires_valid_transaction;
	bool allow_stream_result;

private:
	void CreatePlan(SQLStatement &statement);
	shared_ptr<PreparedStatementData> PrepareSQLStatement(unique_ptr<SQLStatement> statement);
	void PlanPrepare(unique_ptr<SQLStatement> statement);
	void PlanExecute(unique_ptr<SQLStatement> statement);

	// void VerifyQuery(BoundSQLStatement &statement);
	// void VerifyNode(BoundQueryNode &statement);
	// void VerifyExpression(Expression &expr, vector<unique_ptr<Expression>> &copies);

	// bool StatementRequiresValidTransaction(BoundSQLStatement &statement);
};
} // namespace guinsoodb

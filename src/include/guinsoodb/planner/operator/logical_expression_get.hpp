//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/operator/logical_expression_get.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/planner/logical_operator.hpp"

namespace guinsoodb {

//! LogicalExpressionGet represents a scan operation over a set of to-be-executed expressions
class LogicalExpressionGet : public LogicalOperator {
public:
	LogicalExpressionGet(idx_t table_index, vector<LogicalType> types,
	                     vector<vector<unique_ptr<Expression>>> expressions)
	    : LogicalOperator(LogicalOperatorType::LOGICAL_EXPRESSION_GET), table_index(table_index), expr_types(types),
	      expressions(move(expressions)) {
	}

	//! The table index in the current bind context
	idx_t table_index;
	//! The types of the expressions
	vector<LogicalType> expr_types;
	//! The set of expressions
	vector<vector<unique_ptr<Expression>>> expressions;

public:
	vector<ColumnBinding> GetColumnBindings() override {
		return GenerateColumnBindings(table_index, expr_types.size());
	}

protected:
	void ResolveTypes() override {
		// types are resolved in the constructor
		this->types = expr_types;
	}
};
} // namespace guinsoodb

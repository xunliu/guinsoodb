//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/expression_binder/alter_binder.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/column_definition.hpp"
#include "guinsoodb/planner/expression_binder.hpp"

namespace guinsoodb {
//! The ALTER binder is responsible for binding an expression within alter statements
class AlterBinder : public ExpressionBinder {
public:
	AlterBinder(Binder &binder, ClientContext &context, string table, vector<ColumnDefinition> &columns,
	            vector<column_t> &bound_columns, LogicalType target_type);

	string table;
	vector<ColumnDefinition> &columns;
	vector<column_t> &bound_columns;

protected:
	BindResult BindExpression(unique_ptr<ParsedExpression> *expr_ptr, idx_t depth,
	                          bool root_expression = false) override;

	BindResult BindColumn(ColumnRefExpression &expr);

	string UnsupportedAggregateMessage() override;
};

} // namespace guinsoodb

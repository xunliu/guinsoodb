//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/expression_binder/relation_binder.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/planner/expression_binder.hpp"

namespace guinsoodb {

//! The relation binder is a binder used to bind expressions in the relation API
class RelationBinder : public ExpressionBinder {
public:
	RelationBinder(Binder &binder, ClientContext &context, string op);

	string op;

protected:
	BindResult BindExpression(unique_ptr<ParsedExpression> *expr_ptr, idx_t depth,
	                          bool root_expression = false) override;

	string UnsupportedAggregateMessage() override;
};

} // namespace guinsoodb

//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/expression_binder/index_binder.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/planner/expression_binder.hpp"
#include "guinsoodb/common/unordered_map.hpp"

namespace guinsoodb {
class BoundColumnRefExpression;

//! The INDEX binder is responsible for binding an expression within an Index statement
class IndexBinder : public ExpressionBinder {
public:
	IndexBinder(Binder &binder, ClientContext &context);

protected:
	BindResult BindExpression(unique_ptr<ParsedExpression> *expr_ptr, idx_t depth,
	                          bool root_expression = false) override;

	string UnsupportedAggregateMessage() override;
};

} // namespace guinsoodb

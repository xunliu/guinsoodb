//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/expression_binder/having_binder.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/planner/expression_binder/select_binder.hpp"

namespace guinsoodb {

//! The HAVING binder is responsible for binding an expression within the HAVING clause of a SQL statement
class HavingBinder : public SelectBinder {
public:
	HavingBinder(Binder &binder, ClientContext &context, BoundSelectNode &node, BoundGroupInformation &info,
	             unordered_map<string, idx_t> &alias_map);

	unordered_map<string, idx_t> &alias_map;
	bool in_alias;

protected:
	BindResult BindExpression(unique_ptr<ParsedExpression> *expr_ptr, idx_t depth,
	                          bool root_expression = false) override;
};

} // namespace guinsoodb

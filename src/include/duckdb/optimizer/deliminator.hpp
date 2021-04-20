//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/optimizer/deliminator.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/planner/logical_operator.hpp"
#include "guinsoodb/planner/logical_operator_visitor.hpp"
#include "guinsoodb/planner/column_binding_map.hpp"
#include "guinsoodb/parser/expression_map.hpp"

namespace guinsoodb {

class Optimizer;
class DeliminatorPlanUpdater;

//! The Deliminator optimizer traverses the logical operator tree and removes any redundant DelimGets/DelimJoins
class Deliminator {
public:
	Deliminator() {
	}
	//! Perform DelimJoin elimination
	unique_ptr<LogicalOperator> Optimize(unique_ptr<LogicalOperator> op);

private:
	//! Find Joins with a DelimGet that can be removed
	void FindCandidates(unique_ptr<LogicalOperator> *op_ptr, vector<unique_ptr<LogicalOperator> *> &candidates);
	//! Try to remove a Join with a DelimGet, returns true if it was successful
	bool RemoveCandidate(unique_ptr<LogicalOperator> *op_ptr, DeliminatorPlanUpdater &updater);
	//! Replace references to a removed DelimGet, remove DelimJoins if all their DelimGets are gone
	void UpdatePlan(LogicalOperator &op, expression_map_t<Expression *> &expr_map,
	                column_binding_map_t<bool> &projection_map);
	//! Whether the operator has one or more children of type DELIM_GET
	bool HasChildDelimGet(LogicalOperator &op);
};

} // namespace guinsoodb

#include "guinsoodb/planner/binder.hpp"
#include "guinsoodb/planner/tableref/bound_expressionlistref.hpp"
#include "guinsoodb/planner/operator/logical_expression_get.hpp"
#include "guinsoodb/planner/operator/logical_dummy_scan.hpp"

namespace guinsoodb {

unique_ptr<LogicalOperator> Binder::CreatePlan(BoundExpressionListRef &ref) {
	auto root = make_unique_base<LogicalOperator, LogicalDummyScan>(0);
	// values list, first plan any subqueries in the list
	for (auto &expr_list : ref.values) {
		for (auto &expr : expr_list) {
			PlanSubqueries(&expr, &root);
		}
	}
	// now create a LogicalExpressionGet from the set of expressions
	// fetch the types
	vector<LogicalType> types;
	for (auto &expr : ref.values[0]) {
		types.push_back(expr->return_type);
	}
	auto expr_get = make_unique<LogicalExpressionGet>(ref.bind_index, types, move(ref.values));
	expr_get->AddChild(move(root));
	return move(expr_get);
}

} // namespace guinsoodb

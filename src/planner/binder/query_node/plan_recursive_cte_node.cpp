#include "guinsoodb/planner/expression/bound_cast_expression.hpp"
#include "guinsoodb/planner/binder.hpp"
#include "guinsoodb/planner/operator/logical_projection.hpp"
#include "guinsoodb/planner/operator/logical_recursive_cte.hpp"
#include "guinsoodb/planner/operator/logical_set_operation.hpp"
#include "guinsoodb/planner/query_node/bound_recursive_cte_node.hpp"

namespace guinsoodb {

unique_ptr<LogicalOperator> Binder::CreatePlan(BoundRecursiveCTENode &node) {
	// Generate the logical plan for the left and right sides of the set operation
	node.left_binder->plan_subquery = plan_subquery;
	node.right_binder->plan_subquery = plan_subquery;

	auto left_node = node.left_binder->CreatePlan(*node.left);
	auto right_node = node.right_binder->CreatePlan(*node.right);

	// check if there are any unplanned subqueries left in either child
	has_unplanned_subqueries =
	    node.left_binder->has_unplanned_subqueries || node.right_binder->has_unplanned_subqueries;

	// for both the left and right sides, cast them to the same types
	left_node = CastLogicalOperatorToTypes(node.left->types, node.types, move(left_node));
	right_node = CastLogicalOperatorToTypes(node.right->types, node.types, move(right_node));

	if (node.right_binder->bind_context.cte_references[node.ctename] == nullptr) {
		auto root = make_unique<LogicalSetOperation>(node.setop_index, node.types.size(), move(left_node),
		                                             move(right_node), LogicalOperatorType::LOGICAL_UNION);
		return VisitQueryNode(node, move(root));
	}
	auto root = make_unique<LogicalRecursiveCTE>(node.setop_index, node.types.size(), node.union_all, move(left_node),
	                                             move(right_node), LogicalOperatorType::LOGICAL_RECURSIVE_CTE);

	return VisitQueryNode(node, move(root));
}

} // namespace guinsoodb

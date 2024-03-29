#include "guinsoodb/execution/operator/scan/physical_expression_scan.hpp"
#include "guinsoodb/execution/physical_plan_generator.hpp"
#include "guinsoodb/planner/operator/logical_expression_get.hpp"

namespace guinsoodb {

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalExpressionGet &op) {
	D_ASSERT(op.children.size() == 1);
	auto plan = CreatePlan(*op.children[0]);

	auto expr_scan = make_unique<PhysicalExpressionScan>(op.types, move(op.expressions), op.estimated_cardinality);
	expr_scan->children.push_back(move(plan));
	return move(expr_scan);
}

} // namespace guinsoodb

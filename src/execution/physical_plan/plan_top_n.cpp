#include "guinsoodb/execution/operator/order/physical_top_n.hpp"
#include "guinsoodb/execution/physical_plan_generator.hpp"
#include "guinsoodb/planner/operator/logical_top_n.hpp"

namespace guinsoodb {

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalTopN &op) {
	D_ASSERT(op.children.size() == 1);

	auto plan = CreatePlan(*op.children[0]);

	auto top_n = make_unique<PhysicalTopN>(op.types, move(op.orders), op.limit, op.offset, op.estimated_cardinality);
	top_n->children.push_back(move(plan));
	return move(top_n);
}

} // namespace guinsoodb

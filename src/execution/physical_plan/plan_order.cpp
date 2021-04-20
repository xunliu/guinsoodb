#include "guinsoodb/execution/operator/order/physical_order.hpp"
#include "guinsoodb/execution/physical_plan_generator.hpp"
#include "guinsoodb/planner/operator/logical_order.hpp"

namespace guinsoodb {

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalOrder &op) {
	D_ASSERT(op.children.size() == 1);

	auto plan = CreatePlan(*op.children[0]);
	if (!op.orders.empty()) {
		auto order = make_unique<PhysicalOrder>(op.types, move(op.orders), op.estimated_cardinality);
		order->children.push_back(move(plan));
		plan = move(order);
	}
	return plan;
}

} // namespace guinsoodb

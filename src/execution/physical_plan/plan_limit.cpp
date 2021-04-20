#include "guinsoodb/execution/operator/helper/physical_limit.hpp"
#include "guinsoodb/execution/physical_plan_generator.hpp"
#include "guinsoodb/planner/operator/logical_limit.hpp"

namespace guinsoodb {

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalLimit &op) {
	D_ASSERT(op.children.size() == 1);

	auto plan = CreatePlan(*op.children[0]);

	auto limit = make_unique<PhysicalLimit>(op.types, op.limit_val, op.offset_val, move(op.limit), move(op.offset),
	                                        op.estimated_cardinality);
	limit->children.push_back(move(plan));
	return move(limit);
}

} // namespace guinsoodb

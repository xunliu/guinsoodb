#include "guinsoodb/execution/operator/join/physical_cross_product.hpp"
#include "guinsoodb/execution/physical_plan_generator.hpp"
#include "guinsoodb/planner/operator/logical_cross_product.hpp"

namespace guinsoodb {

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalCrossProduct &op) {
	D_ASSERT(op.children.size() == 2);

	auto left = CreatePlan(*op.children[0]);
	auto right = CreatePlan(*op.children[1]);
	return make_unique<PhysicalCrossProduct>(op.types, move(left), move(right), op.estimated_cardinality);
}

} // namespace guinsoodb

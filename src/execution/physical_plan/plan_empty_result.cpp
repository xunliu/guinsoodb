#include "guinsoodb/execution/operator/scan/physical_empty_result.hpp"
#include "guinsoodb/execution/physical_plan_generator.hpp"
#include "guinsoodb/planner/operator/logical_empty_result.hpp"

namespace guinsoodb {

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalEmptyResult &op) {
	D_ASSERT(op.children.size() == 0);
	return make_unique<PhysicalEmptyResult>(op.types, op.estimated_cardinality);
}

} // namespace guinsoodb

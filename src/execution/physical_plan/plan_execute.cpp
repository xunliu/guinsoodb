#include "guinsoodb/execution/operator/helper/physical_execute.hpp"
#include "guinsoodb/execution/physical_plan_generator.hpp"
#include "guinsoodb/planner/operator/logical_execute.hpp"

namespace guinsoodb {

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalExecute &op) {
	D_ASSERT(op.children.size() == 0);
	return make_unique<PhysicalExecute>(op.prepared->plan.get());
}

} // namespace guinsoodb

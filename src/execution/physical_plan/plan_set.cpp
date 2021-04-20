#include "guinsoodb/execution/physical_plan_generator.hpp"
#include "guinsoodb/planner/operator/logical_set.hpp"
#include "guinsoodb/execution/operator/helper/physical_set.hpp"

namespace guinsoodb {

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalSet &op) {
	return make_unique<PhysicalSet>(op.name, op.value, op.estimated_cardinality);
}

} // namespace guinsoodb

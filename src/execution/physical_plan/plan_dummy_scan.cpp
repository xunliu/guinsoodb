#include "guinsoodb/execution/operator/scan/physical_dummy_scan.hpp"
#include "guinsoodb/execution/physical_plan_generator.hpp"
#include "guinsoodb/planner/operator/logical_dummy_scan.hpp"

namespace guinsoodb {

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalDummyScan &op) {
	D_ASSERT(op.children.size() == 0);
	return make_unique<PhysicalDummyScan>(op.types, op.estimated_cardinality);
}

} // namespace guinsoodb

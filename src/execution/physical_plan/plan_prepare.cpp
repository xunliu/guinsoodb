#include "guinsoodb/execution/operator/scan/physical_dummy_scan.hpp"
#include "guinsoodb/execution/physical_plan_generator.hpp"
#include "guinsoodb/planner/operator/logical_prepare.hpp"
#include "guinsoodb/execution/operator/helper/physical_prepare.hpp"

namespace guinsoodb {

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalPrepare &op) {
	D_ASSERT(op.children.size() == 1);

	// generate physical plan
	auto plan = CreatePlan(*op.children[0]);
	op.prepared->types = plan->types;
	op.prepared->plan = move(plan);

	return make_unique<PhysicalPrepare>(op.name, move(op.prepared), op.estimated_cardinality);
}

} // namespace guinsoodb

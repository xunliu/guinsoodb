#include "guinsoodb/execution/operator/projection/physical_unnest.hpp"
#include "guinsoodb/execution/physical_plan_generator.hpp"
#include "guinsoodb/planner/operator/logical_unnest.hpp"

namespace guinsoodb {

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalUnnest &op) {
	D_ASSERT(op.children.size() == 1);
	auto plan = CreatePlan(*op.children[0]);
	auto unnest = make_unique<PhysicalUnnest>(op.types, move(op.expressions), op.estimated_cardinality);
	unnest->children.push_back(move(plan));
	return move(unnest);
}

} // namespace guinsoodb

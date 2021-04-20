#include "guinsoodb/execution/physical_plan_generator.hpp"
#include "guinsoodb/execution/operator/persistent/physical_copy_to_file.hpp"
#include "guinsoodb/planner/operator/logical_copy_to_file.hpp"

namespace guinsoodb {

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalCopyToFile &op) {
	auto plan = CreatePlan(*op.children[0]);
	// COPY from select statement to file
	auto copy = make_unique<PhysicalCopyToFile>(op.types, op.function, move(op.bind_data), op.estimated_cardinality);

	copy->children.push_back(move(plan));
	return move(copy);
}

} // namespace guinsoodb

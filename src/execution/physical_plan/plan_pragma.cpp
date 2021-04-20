#include "guinsoodb/execution/physical_plan_generator.hpp"
#include "guinsoodb/planner/operator/logical_pragma.hpp"

#include "guinsoodb/execution/operator/helper/physical_pragma.hpp"
namespace guinsoodb {

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalPragma &op) {
	return make_unique<PhysicalPragma>(op.function, op.info, op.estimated_cardinality);
}

} // namespace guinsoodb

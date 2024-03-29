#include "guinsoodb/execution/physical_plan_generator.hpp"
#include "guinsoodb/planner/logical_operator.hpp"
#include "guinsoodb/planner/operator/logical_create.hpp"

#include "guinsoodb/execution/operator/schema/physical_create_schema.hpp"
#include "guinsoodb/execution/operator/schema/physical_create_sequence.hpp"
#include "guinsoodb/execution/operator/schema/physical_create_view.hpp"
#include "guinsoodb/execution/operator/schema/physical_create_function.hpp"

namespace guinsoodb {

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalCreate &op) {
	switch (op.type) {
	case LogicalOperatorType::LOGICAL_CREATE_SEQUENCE:
		return make_unique<PhysicalCreateSequence>(unique_ptr_cast<CreateInfo, CreateSequenceInfo>(move(op.info)),
		                                           op.estimated_cardinality);
	case LogicalOperatorType::LOGICAL_CREATE_VIEW:
		return make_unique<PhysicalCreateView>(unique_ptr_cast<CreateInfo, CreateViewInfo>(move(op.info)),
		                                       op.estimated_cardinality);
	case LogicalOperatorType::LOGICAL_CREATE_SCHEMA:
		return make_unique<PhysicalCreateSchema>(unique_ptr_cast<CreateInfo, CreateSchemaInfo>(move(op.info)),
		                                         op.estimated_cardinality);
	case LogicalOperatorType::LOGICAL_CREATE_MACRO:
		return make_unique<PhysicalCreateFunction>(unique_ptr_cast<CreateInfo, CreateMacroInfo>(move(op.info)),
		                                           op.estimated_cardinality);
	default:
		throw NotImplementedException("Unimplemented type for logical simple create");
	}
}

} // namespace guinsoodb

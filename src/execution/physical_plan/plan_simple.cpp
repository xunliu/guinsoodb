#include "guinsoodb/execution/physical_plan_generator.hpp"
#include "guinsoodb/planner/logical_operator.hpp"
#include "guinsoodb/planner/operator/logical_simple.hpp"

#include "guinsoodb/execution/operator/helper/physical_transaction.hpp"
#include "guinsoodb/execution/operator/schema/physical_alter.hpp"
#include "guinsoodb/execution/operator/schema/physical_create_schema.hpp"
#include "guinsoodb/execution/operator/schema/physical_create_sequence.hpp"
#include "guinsoodb/execution/operator/schema/physical_create_view.hpp"
#include "guinsoodb/execution/operator/schema/physical_drop.hpp"
#include "guinsoodb/execution/operator/helper/physical_vacuum.hpp"
#include "guinsoodb/execution/operator/helper/physical_load.hpp"

namespace guinsoodb {

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalSimple &op) {
	switch (op.type) {
	case LogicalOperatorType::LOGICAL_ALTER:
		return make_unique<PhysicalAlter>(unique_ptr_cast<ParseInfo, AlterInfo>(move(op.info)),
		                                  op.estimated_cardinality);
	case LogicalOperatorType::LOGICAL_DROP:
		return make_unique<PhysicalDrop>(unique_ptr_cast<ParseInfo, DropInfo>(move(op.info)), op.estimated_cardinality);
	case LogicalOperatorType::LOGICAL_TRANSACTION:
		return make_unique<PhysicalTransaction>(unique_ptr_cast<ParseInfo, TransactionInfo>(move(op.info)),
		                                        op.estimated_cardinality);
	case LogicalOperatorType::LOGICAL_VACUUM:
		return make_unique<PhysicalVacuum>(unique_ptr_cast<ParseInfo, VacuumInfo>(move(op.info)),
		                                   op.estimated_cardinality);
	case LogicalOperatorType::LOGICAL_LOAD:
		return make_unique<PhysicalLoad>(unique_ptr_cast<ParseInfo, LoadInfo>(move(op.info)), op.estimated_cardinality);
	default:
		throw NotImplementedException("Unimplemented type for logical simple operator");
	}
}

} // namespace guinsoodb

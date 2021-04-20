#include "guinsoodb/catalog/catalog_entry/table_catalog_entry.hpp"
#include "guinsoodb/execution/operator/persistent/physical_update.hpp"
#include "guinsoodb/execution/physical_plan_generator.hpp"
#include "guinsoodb/planner/operator/logical_update.hpp"

namespace guinsoodb {

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalUpdate &op) {
	D_ASSERT(op.children.size() == 1);

	auto plan = CreatePlan(*op.children[0]);

	dependencies.insert(op.table);
	auto update = make_unique<PhysicalUpdate>(op.types, *op.table, *op.table->storage, op.columns, move(op.expressions),
	                                          move(op.bound_defaults), op.estimated_cardinality);
	update->is_index_update = op.is_index_update;
	update->children.push_back(move(plan));
	return move(update);
}

} // namespace guinsoodb

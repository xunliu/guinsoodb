#include "guinsoodb/catalog/catalog_entry/table_catalog_entry.hpp"
#include "guinsoodb/execution/operator/persistent/physical_insert.hpp"
#include "guinsoodb/execution/physical_plan_generator.hpp"
#include "guinsoodb/planner/operator/logical_insert.hpp"

namespace guinsoodb {

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalInsert &op) {
	unique_ptr<PhysicalOperator> plan;
	if (!op.children.empty()) {
		D_ASSERT(op.children.size() == 1);
		plan = CreatePlan(*op.children[0]);
	}

	dependencies.insert(op.table);
	auto insert = make_unique<PhysicalInsert>(op.types, op.table, op.column_index_map, move(op.bound_defaults),
	                                          op.estimated_cardinality);
	if (plan) {
		insert->children.push_back(move(plan));
	}
	return move(insert);
}

} // namespace guinsoodb

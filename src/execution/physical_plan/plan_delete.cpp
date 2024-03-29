#include "guinsoodb/catalog/catalog_entry/table_catalog_entry.hpp"
#include "guinsoodb/execution/operator/persistent/physical_delete.hpp"
#include "guinsoodb/execution/physical_plan_generator.hpp"
#include "guinsoodb/planner/expression/bound_reference_expression.hpp"
#include "guinsoodb/planner/operator/logical_delete.hpp"

namespace guinsoodb {

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalDelete &op) {
	D_ASSERT(op.children.size() == 1);
	D_ASSERT(op.expressions.size() == 1);
	D_ASSERT(op.expressions[0]->type == ExpressionType::BOUND_REF);

	auto plan = CreatePlan(*op.children[0]);

	// get the index of the row_id column
	auto &bound_ref = (BoundReferenceExpression &)*op.expressions[0];

	dependencies.insert(op.table);
	auto del =
	    make_unique<PhysicalDelete>(op.types, *op.table, *op.table->storage, bound_ref.index, op.estimated_cardinality);
	del->children.push_back(move(plan));
	return move(del);
}

} // namespace guinsoodb

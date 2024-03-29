#include "guinsoodb/execution/operator/filter/physical_filter.hpp"
#include "guinsoodb/execution/operator/projection/physical_projection.hpp"
#include "guinsoodb/execution/physical_plan_generator.hpp"
#include "guinsoodb/optimizer/matcher/expression_matcher.hpp"
#include "guinsoodb/planner/expression/bound_comparison_expression.hpp"
#include "guinsoodb/planner/expression/bound_constant_expression.hpp"
#include "guinsoodb/planner/expression/bound_reference_expression.hpp"
#include "guinsoodb/planner/operator/logical_filter.hpp"
#include "guinsoodb/planner/operator/logical_get.hpp"

namespace guinsoodb {

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalFilter &op) {
	D_ASSERT(op.children.size() == 1);
	unique_ptr<PhysicalOperator> plan = CreatePlan(*op.children[0]);
	if (!op.expressions.empty()) {
		D_ASSERT(plan->types.size() > 0);
		// create a filter if there is anything to filter
		auto filter = make_unique<PhysicalFilter>(plan->types, move(op.expressions), op.estimated_cardinality);
		filter->children.push_back(move(plan));
		plan = move(filter);
	}
	if (!op.projection_map.empty()) {
		// there is a projection map, generate a physical projection
		vector<unique_ptr<Expression>> select_list;
		for (idx_t i = 0; i < op.projection_map.size(); i++) {
			select_list.push_back(make_unique<BoundReferenceExpression>(op.types[i], op.projection_map[i]));
		}
		auto proj = make_unique<PhysicalProjection>(op.types, move(select_list), op.estimated_cardinality);
		proj->children.push_back(move(plan));
		plan = move(proj);
	}
	return plan;
}

} // namespace guinsoodb

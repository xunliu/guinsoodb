#include "guinsoodb/execution/operator/aggregate/physical_hash_aggregate.hpp"
#include "guinsoodb/execution/operator/projection/physical_projection.hpp"
#include "guinsoodb/execution/physical_plan_generator.hpp"
#include "guinsoodb/function/aggregate/distributive_functions.hpp"
#include "guinsoodb/planner/expression/bound_aggregate_expression.hpp"
#include "guinsoodb/planner/expression/bound_reference_expression.hpp"
#include "guinsoodb/planner/operator/logical_distinct.hpp"

namespace guinsoodb {

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreateDistinctOn(unique_ptr<PhysicalOperator> child,
                                                                     vector<unique_ptr<Expression>> distinct_targets) {
	D_ASSERT(child);
	D_ASSERT(!distinct_targets.empty());

	auto &types = child->GetTypes();
	vector<unique_ptr<Expression>> groups, aggregates, projections;
	idx_t group_count = distinct_targets.size();
	unordered_map<idx_t, idx_t> group_by_references;
	vector<LogicalType> aggregate_types;
	// creates one group per distinct_target
	for (idx_t i = 0; i < distinct_targets.size(); i++) {
		auto &target = distinct_targets[i];
		if (target->type == ExpressionType::BOUND_REF) {
			auto &bound_ref = (BoundReferenceExpression &)*target;
			group_by_references[bound_ref.index] = i;
		}
		aggregate_types.push_back(target->return_type);
		groups.push_back(move(target));
	}
	bool requires_projection = false;
	if (types.size() != group_count) {
		requires_projection = true;
	}
	// we need to create one aggregate per column in the select_list
	for (idx_t i = 0; i < types.size(); ++i) {
		auto logical_type = types[i];
		// check if we can directly refer to a group, or if we need to push an aggregate with FIRST
		auto entry = group_by_references.find(i);
		if (entry != group_by_references.end()) {
			auto group_index = entry->second;
			// entry is found: can directly refer to a group
			projections.push_back(make_unique<BoundReferenceExpression>(logical_type, group_index));
			if (group_index != i) {
				// we require a projection only if this group element is out of order
				requires_projection = true;
			}
		} else {
			// entry is not one of the groups: need to push a FIRST aggregate
			auto bound = make_unique<BoundReferenceExpression>(logical_type, i);
			vector<unique_ptr<Expression>> first_children;
			first_children.push_back(move(bound));
			auto first_aggregate = AggregateFunction::BindAggregateFunction(
			    context, FirstFun::GetFunction(logical_type), move(first_children), nullptr, false);
			// add the projection
			projections.push_back(make_unique<BoundReferenceExpression>(logical_type, group_count + aggregates.size()));
			// push it to the list of aggregates
			aggregate_types.push_back(logical_type);
			aggregates.push_back(move(first_aggregate));
			requires_projection = true;
		}
	}

	child = ExtractAggregateExpressions(move(child), aggregates, groups);

	// we add a physical hash aggregation in the plan to select the distinct groups
	auto groupby = make_unique<PhysicalHashAggregate>(context, aggregate_types, move(aggregates), move(groups),
	                                                  child->estimated_cardinality);
	groupby->children.push_back(move(child));
	if (!requires_projection) {
		return move(groupby);
	}

	// we add a physical projection on top of the aggregation to project all members in the select list
	auto aggr_projection = make_unique<PhysicalProjection>(types, move(projections), groupby->estimated_cardinality);
	aggr_projection->children.push_back(move(groupby));
	return move(aggr_projection);
}

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalDistinct &op) {
	D_ASSERT(op.children.size() == 1);
	auto plan = CreatePlan(*op.children[0]);
	return CreateDistinctOn(move(plan), move(op.distinct_targets));
}

} // namespace guinsoodb

#include "guinsoodb/planner/binder.hpp"
#include "guinsoodb/planner/tableref/bound_subqueryref.hpp"

namespace guinsoodb {

unique_ptr<LogicalOperator> Binder::CreatePlan(BoundSubqueryRef &ref) {
	// generate the logical plan for the subquery
	// this happens separately from the current LogicalPlan generation
	ref.binder->plan_subquery = plan_subquery;
	auto subquery = ref.binder->CreatePlan(*ref.subquery);
	if (ref.binder->has_unplanned_subqueries) {
		has_unplanned_subqueries = true;
	}
	return subquery;
}

} // namespace guinsoodb

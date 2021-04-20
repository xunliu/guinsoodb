#include "guinsoodb/optimizer/filter_pullup.hpp"
#include "guinsoodb/planner/operator/logical_filter.hpp"
#include "guinsoodb/planner/expression/bound_comparison_expression.hpp"
#include "guinsoodb/planner/expression_iterator.hpp"
#include "guinsoodb/planner/expression/bound_between_expression.hpp"

namespace guinsoodb {

unique_ptr<LogicalOperator> FilterPullup::PullupFilter(unique_ptr<LogicalOperator> op) {
	D_ASSERT(op->type == LogicalOperatorType::LOGICAL_FILTER);

	if (can_pullup) {
		unique_ptr<LogicalOperator> child = move(op->children[0]);
		child = Rewrite(move(child));
		// moving filter's expressions
		for (idx_t i = 0; i < op->expressions.size(); ++i) {
			filters_expr_pullup.push_back(move(op->expressions[i]));
		}
		return child;
	}
	op->children[0] = Rewrite(move(op->children[0]));
	return op;
}

} // namespace guinsoodb

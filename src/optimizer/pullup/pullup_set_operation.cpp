#include "guinsoodb/optimizer/filter_pullup.hpp"
#include "guinsoodb/planner/operator/logical_set_operation.hpp"
#include "guinsoodb/planner/expression/bound_columnref_expression.hpp"
#include "guinsoodb/planner/expression_iterator.hpp"

namespace guinsoodb {

static void ReplaceFilterTableIndex(Expression &expr, LogicalSetOperation &setop) {
	if (expr.type == ExpressionType::BOUND_COLUMN_REF) {
		auto &colref = (BoundColumnRefExpression &)expr;
		D_ASSERT(colref.depth == 0);

		colref.binding.table_index = setop.table_index;
		return;
	}
	ExpressionIterator::EnumerateChildren(expr, [&](Expression &child) { ReplaceFilterTableIndex(child, setop); });
}

unique_ptr<LogicalOperator> FilterPullup::PullupSetOperation(unique_ptr<LogicalOperator> op) {
	D_ASSERT(op->type == LogicalOperatorType::LOGICAL_INTERSECT || op->type == LogicalOperatorType::LOGICAL_EXCEPT);
	can_add_column = false;
	can_pullup = true;
	if (op->type == LogicalOperatorType::LOGICAL_INTERSECT) {
		op = PullupBothSide(move(op));
	} else {
		// EXCEPT only pull ups from LHS
		op = PullupFromLeft(move(op));
	}
	if (op->type == LogicalOperatorType::LOGICAL_FILTER) {
		auto &filter = (LogicalFilter &)*op;
		auto &setop = (LogicalSetOperation &)*filter.children[0];
		for (idx_t i = 0; i < filter.expressions.size(); ++i) {
			ReplaceFilterTableIndex(*filter.expressions[i], setop);
		}
	}
	return op;
}

} // namespace guinsoodb

#include "guinsoodb/planner/expression_binder/relation_binder.hpp"

namespace guinsoodb {

RelationBinder::RelationBinder(Binder &binder, ClientContext &context, string op)
    : ExpressionBinder(binder, context), op(move(op)) {
}

BindResult RelationBinder::BindExpression(unique_ptr<ParsedExpression> *expr_ptr, idx_t depth, bool root_expression) {
	auto &expr = **expr_ptr;
	switch (expr.expression_class) {
	case ExpressionClass::AGGREGATE:
		return BindResult("aggregate functions are not allowed in " + op);
	case ExpressionClass::DEFAULT:
		return BindResult(op + " cannot contain DEFAULT clause");
	case ExpressionClass::SUBQUERY:
		return BindResult("subqueries are not allowed in " + op);
	case ExpressionClass::WINDOW:
		return BindResult("window functions are not allowed in " + op);
	default:
		return ExpressionBinder::BindExpression(expr_ptr, depth);
	}
}

string RelationBinder::UnsupportedAggregateMessage() {
	return "aggregate functions are not allowed in " + op;
}

} // namespace guinsoodb

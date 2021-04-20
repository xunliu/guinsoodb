#include "guinsoodb/parser/expression/collate_expression.hpp"
#include "guinsoodb/planner/expression_binder.hpp"

namespace guinsoodb {

BindResult ExpressionBinder::BindExpression(CollateExpression &expr, idx_t depth) {
	// first try to bind the child of the cast expression
	string error = Bind(&expr.child, depth);
	if (!error.empty()) {
		return BindResult(error);
	}
	auto &child = (BoundExpression &)*expr.child;
	if (child.expr->return_type.id() != LogicalTypeId::VARCHAR) {
		throw BinderException("collations are only supported for type varchar");
	}
	child.expr->return_type = LogicalType(LogicalTypeId::VARCHAR, expr.collation);
	return BindResult(move(child.expr));
}

} // namespace guinsoodb

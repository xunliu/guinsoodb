#include "guinsoodb/parser/expression/constant_expression.hpp"
#include "guinsoodb/planner/expression/bound_constant_expression.hpp"
#include "guinsoodb/planner/expression_binder.hpp"

namespace guinsoodb {

BindResult ExpressionBinder::BindExpression(ConstantExpression &expr, idx_t depth) {
	return BindResult(make_unique<BoundConstantExpression>(expr.value));
}

} // namespace guinsoodb

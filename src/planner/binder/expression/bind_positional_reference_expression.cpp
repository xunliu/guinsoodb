#include "guinsoodb/parser/expression/positional_reference_expression.hpp"
#include "guinsoodb/planner/expression_binder.hpp"
#include "guinsoodb/planner/binder.hpp"

namespace guinsoodb {

BindResult ExpressionBinder::BindExpression(PositionalReferenceExpression &ref, idx_t depth) {
	if (depth != 0) {
		return BindResult("Positional reference expression could not be bound");
	}
	return binder.bind_context.BindColumn(ref, depth);
}

} // namespace guinsoodb

#include "guinsoodb/common/vector_operations/vector_operations.hpp"
#include "guinsoodb/execution/expression_executor.hpp"
#include "guinsoodb/planner/expression/bound_constant_expression.hpp"

namespace guinsoodb {

unique_ptr<ExpressionState> ExpressionExecutor::InitializeState(BoundConstantExpression &expr,
                                                                ExpressionExecutorState &root) {
	auto result = make_unique<ExpressionState>(expr, root);
	result->Finalize();
	return result;
}

void ExpressionExecutor::Execute(BoundConstantExpression &expr, ExpressionState *state, const SelectionVector *sel,
                                 idx_t count, Vector &result) {
	D_ASSERT(expr.value.type() == expr.return_type);
	result.Reference(expr.value);
}

} // namespace guinsoodb

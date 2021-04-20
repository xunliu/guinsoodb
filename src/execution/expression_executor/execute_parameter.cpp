#include "guinsoodb/common/vector_operations/vector_operations.hpp"
#include "guinsoodb/execution/expression_executor.hpp"
#include "guinsoodb/planner/expression/bound_parameter_expression.hpp"

namespace guinsoodb {

unique_ptr<ExpressionState> ExpressionExecutor::InitializeState(BoundParameterExpression &expr,
                                                                ExpressionExecutorState &root) {
	auto result = make_unique<ExpressionState>(expr, root);
	result->Finalize();
	return result;
}

void ExpressionExecutor::Execute(BoundParameterExpression &expr, ExpressionState *state, const SelectionVector *sel,
                                 idx_t count, Vector &result) {
	D_ASSERT(expr.value);
	D_ASSERT(expr.value->type() == expr.return_type);
	result.Reference(*expr.value);
}

} // namespace guinsoodb

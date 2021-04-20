#include "guinsoodb/execution/expression_executor_state.hpp"
#include "guinsoodb/execution/expression_executor.hpp"
#include "guinsoodb/planner/expression.hpp"

namespace guinsoodb {

void ExpressionState::AddChild(Expression *expr) {
	types.push_back(expr->return_type);
	child_states.push_back(ExpressionExecutor::InitializeState(*expr, root));
}

void ExpressionState::Finalize() {
	if (!types.empty()) {
		intermediate_chunk.Initialize(types);
	}
}
ExpressionState::ExpressionState(Expression &expr, ExpressionExecutorState &root)
    : expr(expr), root(root), name(expr.ToString()) {
}

} // namespace guinsoodb

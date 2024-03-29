#include "guinsoodb/function/scalar/generic_functions.hpp"
#include "guinsoodb/planner/expression/bound_function_expression.hpp"

namespace guinsoodb {

static void AliasFunction(DataChunk &args, ExpressionState &state, Vector &result) {
	auto &func_expr = (BoundFunctionExpression &)state.expr;
	Value v(state.expr.alias.empty() ? func_expr.children[0]->GetName() : state.expr.alias);
	result.Reference(v);
}

void AliasFun::RegisterFunction(BuiltinFunctions &set) {
	set.AddFunction(ScalarFunction("alias", {LogicalType::ANY}, LogicalType::VARCHAR, AliasFunction));
}

} // namespace guinsoodb

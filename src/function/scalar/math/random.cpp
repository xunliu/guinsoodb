#include "guinsoodb/function/scalar/math_functions.hpp"
#include "guinsoodb/common/vector_operations/vector_operations.hpp"
#include "guinsoodb/execution/expression_executor.hpp"
#include "guinsoodb/main/client_context.hpp"
#include "guinsoodb/planner/expression/bound_function_expression.hpp"
#include <random>

namespace guinsoodb {

struct RandomBindData : public FunctionData {
	ClientContext &context;
	std::uniform_real_distribution<double> dist;

	RandomBindData(ClientContext &context, std::uniform_real_distribution<double> dist) : context(context), dist(dist) {
	}

	unique_ptr<FunctionData> Copy() override {
		return make_unique<RandomBindData>(context, dist);
	}
};

static void RandomFunction(DataChunk &args, ExpressionState &state, Vector &result) {
	D_ASSERT(args.ColumnCount() == 0);
	auto &func_expr = (BoundFunctionExpression &)state.expr;
	auto &info = (RandomBindData &)*func_expr.bind_info;

	result.SetVectorType(VectorType::FLAT_VECTOR);
	auto result_data = FlatVector::GetData<double>(result);
	for (idx_t i = 0; i < args.size(); i++) {
		result_data[i] = info.dist(info.context.random_engine);
	}
}

unique_ptr<FunctionData> RandomBind(ClientContext &context, ScalarFunction &bound_function,
                                    vector<unique_ptr<Expression>> &arguments) {
	std::uniform_real_distribution<double> dist(0, 1);
	return make_unique<RandomBindData>(context, dist);
}

void RandomFun::RegisterFunction(BuiltinFunctions &set) {
	set.AddFunction(ScalarFunction("random", {}, LogicalType::DOUBLE, RandomFunction, true, RandomBind));
}

} // namespace guinsoodb

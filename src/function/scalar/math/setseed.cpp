#include "guinsoodb/function/scalar/math_functions.hpp"
#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/common/vector_operations/vector_operations.hpp"
#include "guinsoodb/execution/expression_executor.hpp"
#include "guinsoodb/main/client_context.hpp"
#include "guinsoodb/planner/expression/bound_function_expression.hpp"
#include "guinsoodb/common/limits.hpp"

namespace guinsoodb {

struct SetseedBindData : public FunctionData {
	//! The client context for the function call
	ClientContext &context;

	explicit SetseedBindData(ClientContext &context) : context(context) {
	}

	unique_ptr<FunctionData> Copy() override {
		return make_unique<SetseedBindData>(context);
	}
};

static void SetSeedFunction(DataChunk &args, ExpressionState &state, Vector &result) {
	auto &func_expr = (BoundFunctionExpression &)state.expr;
	auto &info = (SetseedBindData &)*func_expr.bind_info;
	auto &input = args.data[0];
	input.Normalify(args.size());

	auto input_seeds = FlatVector::GetData<double>(input);
	uint32_t half_max = NumericLimits<uint32_t>::Maximum() / 2;

	for (idx_t i = 0; i < args.size(); i++) {
		if (input_seeds[i] < -1.0 || input_seeds[i] > 1.0) {
			throw Exception("SETSEED accepts seed values between -1.0 and 1.0, inclusive");
		}
		uint32_t norm_seed = (input_seeds[i] + 1.0) * half_max;
		info.context.random_engine.seed(norm_seed);
	}

	result.SetVectorType(VectorType::CONSTANT_VECTOR);
	ConstantVector::SetNull(result, true);
}

unique_ptr<FunctionData> SetSeedBind(ClientContext &context, ScalarFunction &bound_function,
                                     vector<unique_ptr<Expression>> &arguments) {
	return make_unique<SetseedBindData>(context);
}

void SetseedFun::RegisterFunction(BuiltinFunctions &set) {
	set.AddFunction(
	    ScalarFunction("setseed", {LogicalType::DOUBLE}, LogicalType::SQLNULL, SetSeedFunction, true, SetSeedBind));
}

} // namespace guinsoodb

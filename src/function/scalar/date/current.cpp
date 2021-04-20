#include "guinsoodb/function/scalar/date_functions.hpp"

#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/common/types/timestamp.hpp"
#include "guinsoodb/common/vector_operations/vector_operations.hpp"
#include "guinsoodb/planner/expression/bound_function_expression.hpp"
#include "guinsoodb/main/client_context.hpp"
#include "guinsoodb/transaction/transaction.hpp"

namespace guinsoodb {

struct CurrentBindData : public FunctionData {
	ClientContext &context;

	explicit CurrentBindData(ClientContext &context) : context(context) {
	}

	unique_ptr<FunctionData> Copy() override {
		return make_unique<CurrentBindData>(context);
	}
};

static timestamp_t GetTransactionTimestamp(ExpressionState &state) {
	auto &func_expr = (BoundFunctionExpression &)state.expr;
	auto &info = (CurrentBindData &)*func_expr.bind_info;
	return info.context.ActiveTransaction().start_timestamp;
}

static void CurrentTimeFunction(DataChunk &input, ExpressionState &state, Vector &result) {
	D_ASSERT(input.ColumnCount() == 0);

	auto val = Value::TIME(Timestamp::GetTime(GetTransactionTimestamp(state)));
	result.Reference(val);
}

static void CurrentDateFunction(DataChunk &input, ExpressionState &state, Vector &result) {
	D_ASSERT(input.ColumnCount() == 0);

	auto val = Value::DATE(Timestamp::GetDate(GetTransactionTimestamp(state)));
	result.Reference(val);
}

static void CurrentTimestampFunction(DataChunk &input, ExpressionState &state, Vector &result) {
	D_ASSERT(input.ColumnCount() == 0);

	auto val = Value::TIMESTAMP(GetTransactionTimestamp(state));
	result.Reference(val);
}

unique_ptr<FunctionData> BindCurrentTime(ClientContext &context, ScalarFunction &bound_function,
                                         vector<unique_ptr<Expression>> &arguments) {
	return make_unique<CurrentBindData>(context);
}

void CurrentTimeFun::RegisterFunction(BuiltinFunctions &set) {
	set.AddFunction(ScalarFunction("current_time", {}, LogicalType::TIME, CurrentTimeFunction, false, BindCurrentTime));
}

void CurrentDateFun::RegisterFunction(BuiltinFunctions &set) {
	set.AddFunction(ScalarFunction("current_date", {}, LogicalType::DATE, CurrentDateFunction, false, BindCurrentTime));
}

void CurrentTimestampFun::RegisterFunction(BuiltinFunctions &set) {
	set.AddFunction({"now", "current_timestamp"},
	                ScalarFunction({}, LogicalType::TIMESTAMP, CurrentTimestampFunction, false, BindCurrentTime));
}

} // namespace guinsoodb
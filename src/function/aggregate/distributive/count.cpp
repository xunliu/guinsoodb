#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/common/vector_operations/vector_operations.hpp"
#include "guinsoodb/function/aggregate/distributive_functions.hpp"
#include "guinsoodb/planner/expression/bound_aggregate_expression.hpp"
#include "guinsoodb/storage/statistics/validity_statistics.hpp"

namespace guinsoodb {

struct BaseCountFunction {
	template <class STATE>
	static void Initialize(STATE *state) {
		*state = 0;
	}

	template <class STATE, class OP>
	static void Combine(STATE source, STATE *target) {
		*target += source;
	}

	template <class T, class STATE>
	static void Finalize(Vector &result, FunctionData *, STATE *state, T *target, ValidityMask &mask, idx_t idx) {
		target[idx] = *state;
	}
};

struct CountStarFunction : public BaseCountFunction {
	template <class STATE, class OP>
	static void Operation(STATE *state, FunctionData *bind_data, idx_t idx) {
		*state += 1;
	}

	template <class STATE, class OP>
	static void ConstantOperation(STATE *state, FunctionData *bind_data, idx_t count) {
		*state += count;
	}
};

struct CountFunction : public BaseCountFunction {
	template <class INPUT_TYPE, class STATE, class OP>
	static void Operation(STATE *state, FunctionData *bind_data, INPUT_TYPE *input, ValidityMask &mask, idx_t idx) {
		*state += 1;
	}

	template <class INPUT_TYPE, class STATE, class OP>
	static void ConstantOperation(STATE *state, FunctionData *bind_data, INPUT_TYPE *input, ValidityMask &mask,
	                              idx_t count) {
		*state += count;
	}

	static bool IgnoreNull() {
		return true;
	}
};

AggregateFunction CountFun::GetFunction() {
	return AggregateFunction::UnaryAggregate<int64_t, int64_t, int64_t, CountFunction>(LogicalType(LogicalTypeId::ANY),
	                                                                                   LogicalType::BIGINT);
}

AggregateFunction CountStarFun::GetFunction() {
	return AggregateFunction::NullaryAggregate<int64_t, int64_t, CountStarFunction>(LogicalType::BIGINT);
}

unique_ptr<BaseStatistics> CountPropagateStats(ClientContext &context, BoundAggregateExpression &expr,
                                               FunctionData *bind_data, vector<unique_ptr<BaseStatistics>> &child_stats,
                                               NodeStatistics *node_stats) {
	if (!expr.distinct && child_stats[0] && !child_stats[0]->CanHaveNull()) {
		// count on a column without null values: use count star
		expr.function = CountStarFun::GetFunction();
		expr.function.name = "count_star";
		expr.children.clear();
	}
	return nullptr;
}

void CountFun::RegisterFunction(BuiltinFunctions &set) {
	AggregateFunction count_function = CountFun::GetFunction();
	count_function.statistics = CountPropagateStats;
	AggregateFunctionSet count("count");
	count.AddFunction(count_function);
	// the count function can also be called without arguments
	count_function.arguments.clear();
	count_function.statistics = nullptr;
	count.AddFunction(count_function);
	set.AddFunction(count);
}

void CountStarFun::RegisterFunction(BuiltinFunctions &set) {
	AggregateFunctionSet count("count_star");
	count.AddFunction(CountStarFun::GetFunction());
	set.AddFunction(count);
}

} // namespace guinsoodb

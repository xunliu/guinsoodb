#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/common/vector_operations/vector_operations.hpp"
#include "guinsoodb/function/aggregate/distributive_functions.hpp"
#include "guinsoodb/planner/expression/bound_aggregate_expression.hpp"
#include "guinsoodb/function/function_set.hpp"

namespace guinsoodb {

struct ProductState {
	bool empty;
	double val;
};

struct ProductFunction {
	template <class STATE>
	static void Initialize(STATE *state) {
		state->val = 1;
		state->empty = true;
	}

	template <class STATE, class OP>
	static void Combine(STATE source, STATE *target) {
		target->val *= source.val;
		target->empty = target->empty && source.empty;
	}

	template <class T, class STATE>
	static void Finalize(Vector &result, FunctionData *, STATE *state, T *target, ValidityMask &mask, idx_t idx) {
		if (state->empty) {
			mask.SetInvalid(idx);
			return;
		}
		target[idx] = state->val;
	}
	template <class INPUT_TYPE, class STATE, class OP>
	static void Operation(STATE *state, FunctionData *bind_data, INPUT_TYPE *input, ValidityMask &mask, idx_t idx) {
		if (state->empty) {
			state->empty = false;
		}
		state->val *= input[idx];
	}

	template <class INPUT_TYPE, class STATE, class OP>
	static void ConstantOperation(STATE *state, FunctionData *bind_data, INPUT_TYPE *input, ValidityMask &mask,
	                              idx_t count) {
		for (idx_t i = 0; i < count; i++) {
			Operation<INPUT_TYPE, STATE, OP>(state, bind_data, input, mask, 0);
		}
	}

	static bool IgnoreNull() {
		return true;
	}
};

AggregateFunction ProductFun::GetFunction() {
	return AggregateFunction::UnaryAggregate<ProductState, double, double, ProductFunction>(
	    LogicalType(LogicalTypeId::DOUBLE), LogicalType::DOUBLE);
}

void ProductFun::RegisterFunction(BuiltinFunctions &set) {
	AggregateFunction product_function = ProductFun::GetFunction();
	AggregateFunctionSet product("product");
	product.AddFunction(product_function);
	set.AddFunction(product);
}

} // namespace guinsoodb

#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/common/vector_operations/vector_operations.hpp"
#include "guinsoodb/function/aggregate/distributive_functions.hpp"
#include "guinsoodb/planner/expression/bound_aggregate_expression.hpp"
#include "guinsoodb/function/function_set.hpp"

namespace guinsoodb {

struct BoolState {
	bool empty;
	bool val;
};

struct BoolAndFunFunction {
	template <class STATE>
	static void Initialize(STATE *state) {
		state->val = true;
		state->empty = true;
	}

	template <class STATE, class OP>
	static void Combine(STATE source, STATE *target) {
		target->val = target->val && source.val;
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
		state->empty = false;
		state->val = input[idx] && state->val;
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

struct BoolOrFunFunction {
	template <class STATE>
	static void Initialize(STATE *state) {
		state->val = false;
		state->empty = true;
	}

	template <class STATE, class OP>
	static void Combine(STATE source, STATE *target) {
		target->val = target->val || source.val;
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
		state->empty = false;
		state->val = input[idx] || state->val;
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

AggregateFunction BoolOrFun::GetFunction() {
	return AggregateFunction::UnaryAggregate<BoolState, bool, bool, BoolOrFunFunction>(
	    LogicalType(LogicalTypeId::BOOLEAN), LogicalType::BOOLEAN);
}

AggregateFunction BoolAndFun::GetFunction() {
	return AggregateFunction::UnaryAggregate<BoolState, bool, bool, BoolAndFunFunction>(
	    LogicalType(LogicalTypeId::BOOLEAN), LogicalType::BOOLEAN);
}

void BoolOrFun::RegisterFunction(BuiltinFunctions &set) {
	AggregateFunction bool_or_function = BoolOrFun::GetFunction();
	AggregateFunctionSet bool_or("bool_or");
	bool_or.AddFunction(bool_or_function);
	set.AddFunction(bool_or);
}

void BoolAndFun::RegisterFunction(BuiltinFunctions &set) {
	AggregateFunction bool_and_function = BoolAndFun::GetFunction();
	AggregateFunctionSet bool_and("bool_and");
	bool_and.AddFunction(bool_and_function);
	set.AddFunction(bool_and);
}

} // namespace guinsoodb

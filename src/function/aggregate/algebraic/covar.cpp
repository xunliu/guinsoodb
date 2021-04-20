#include "guinsoodb/function/aggregate/algebraic_functions.hpp"
#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/common/types/null_value.hpp"
#include "guinsoodb/common/vector_operations/vector_operations.hpp"
#include "guinsoodb/function/function_set.hpp"
#include "guinsoodb/function/aggregate/algebraic/covar.hpp"
#include <cmath>

namespace guinsoodb {

void CovarPopFun::RegisterFunction(BuiltinFunctions &set) {
	AggregateFunctionSet covar_pop("covar_pop");
	covar_pop.AddFunction(AggregateFunction::BinaryAggregate<CovarState, double, double, double, CovarPopOperation>(
	    LogicalType::DOUBLE, LogicalType::DOUBLE, LogicalType::DOUBLE));
	set.AddFunction(covar_pop);
}

void CovarSampFun::RegisterFunction(BuiltinFunctions &set) {
	AggregateFunctionSet covar_samp("covar_samp");
	covar_samp.AddFunction(AggregateFunction::BinaryAggregate<CovarState, double, double, double, CovarSampOperation>(
	    LogicalType::DOUBLE, LogicalType::DOUBLE, LogicalType::DOUBLE));
	set.AddFunction(covar_samp);
}

} // namespace guinsoodb

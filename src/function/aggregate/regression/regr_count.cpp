#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/common/vector_operations/vector_operations.hpp"
#include "guinsoodb/function/aggregate/regression_functions.hpp"
#include "guinsoodb/planner/expression/bound_aggregate_expression.hpp"
#include "guinsoodb/function/aggregate/regression/regr_count.hpp"
#include "guinsoodb/function/function_set.hpp"

namespace guinsoodb {

void RegrCountFun::RegisterFunction(BuiltinFunctions &set) {
	AggregateFunctionSet corr("regr_count");
	corr.AddFunction(AggregateFunction::BinaryAggregate<size_t, double, double, uint32_t, RegrCountFunction>(
	    LogicalType::DOUBLE, LogicalType::DOUBLE, LogicalType::UINTEGER));
	set.AddFunction(corr);
}

} // namespace guinsoodb

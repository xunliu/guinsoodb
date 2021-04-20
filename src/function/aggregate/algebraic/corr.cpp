#include "guinsoodb/function/aggregate/algebraic_functions.hpp"
#include "guinsoodb/function/aggregate/algebraic/covar.hpp"
#include "guinsoodb/function/aggregate/algebraic/stddev.hpp"
#include "guinsoodb/function/aggregate/algebraic/corr.hpp"
#include "guinsoodb/function/function_set.hpp"

namespace guinsoodb {
void Corr::RegisterFunction(BuiltinFunctions &set) {
	AggregateFunctionSet corr("corr");
	corr.AddFunction(AggregateFunction::BinaryAggregate<CorrState, double, double, double, CorrOperation>(
	    LogicalType::DOUBLE, LogicalType::DOUBLE, LogicalType::DOUBLE));
	set.AddFunction(corr);
}
} // namespace guinsoodb

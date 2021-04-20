#include "guinsoodb/function/aggregate/algebraic_functions.hpp"
#include "guinsoodb/function/aggregate_function.hpp"

namespace guinsoodb {

void BuiltinFunctions::RegisterAlgebraicAggregates() {
	Register<AvgFun>();

	Register<CovarSampFun>();
	Register<CovarPopFun>();

	Register<StdDevSampFun>();
	Register<StdDevPopFun>();
	Register<VarPopFun>();
	Register<VarSampFun>();
	Register<VarianceFun>();

	Register<Corr>();
}

} // namespace guinsoodb

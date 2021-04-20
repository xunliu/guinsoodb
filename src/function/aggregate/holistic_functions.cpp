#include "guinsoodb/function/aggregate/holistic_functions.hpp"

namespace guinsoodb {

void BuiltinFunctions::RegisterHolisticAggregates() {
	Register<QuantileFun>();
	Register<ApproximateQuantileFun>();
	Register<ReservoirQuantileFun>();
}

} // namespace guinsoodb

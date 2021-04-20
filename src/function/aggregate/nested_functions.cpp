#include "guinsoodb/function/aggregate/nested_functions.hpp"

namespace guinsoodb {

void BuiltinFunctions::RegisterNestedAggregates() {
	Register<ListFun>();
	Register<HistogramFun>();
}

} // namespace guinsoodb

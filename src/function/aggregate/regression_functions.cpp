#include "guinsoodb/function/aggregate/distributive_functions.hpp"
#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/common/types/null_value.hpp"
#include "guinsoodb/common/vector_operations/vector_operations.hpp"
#include "guinsoodb/function/aggregate/regression_functions.hpp"

namespace guinsoodb {

void BuiltinFunctions::RegisterRegressiveAggregates() {
	Register<RegrAvgxFun>();
	Register<RegrAvgyFun>();
	Register<RegrCountFun>();
	Register<RegrSlopeFun>();
	Register<RegrR2Fun>();
	Register<RegrSYYFun>();
	Register<RegrSXXFun>();
	Register<RegrSXYFun>();
	Register<RegrInterceptFun>();
}

} // namespace guinsoodb

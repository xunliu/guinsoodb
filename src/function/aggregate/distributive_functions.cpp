#include "guinsoodb/function/aggregate/distributive_functions.hpp"
#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/common/types/null_value.hpp"
#include "guinsoodb/common/vector_operations/vector_operations.hpp"
#include "guinsoodb/function/aggregate_function.hpp"

namespace guinsoodb {

void BuiltinFunctions::RegisterDistributiveAggregates() {
	Register<BitAndFun>();
	Register<BitOrFun>();
	Register<BitXorFun>();
	Register<CountStarFun>();
	Register<CountFun>();
	Register<FirstFun>();
	Register<MaxFun>();
	Register<MinFun>();
	Register<MaxByFun>();
	Register<MinByFun>();
	Register<SumFun>();
	Register<StringAggFun>();
	Register<ApproxCountDistinctFun>();
	Register<ProductFun>();
	Register<BoolOrFun>();
	Register<BoolAndFun>();
	Register<ModeFun>();
	Register<ArgMinFun>();
	Register<ArgMaxFun>();
	Register<ArbitraryFun>();
	Register<SkewFun>();
	Register<KurtosisFun>();
	Register<EntropyFun>();
}

} // namespace guinsoodb

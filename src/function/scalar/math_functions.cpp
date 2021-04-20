#include "guinsoodb/function/scalar/math_functions.hpp"
#include "guinsoodb/common/exception.hpp"

namespace guinsoodb {

void BuiltinFunctions::RegisterMathFunctions() {
	Register<AbsFun>();
	Register<SignFun>();

	Register<CeilFun>();
	Register<FloorFun>();
	Register<RoundFun>();

	Register<DegreesFun>();
	Register<RadiansFun>();

	Register<CbrtFun>();
	Register<ExpFun>();
	Register<Log2Fun>();
	Register<Log10Fun>();
	Register<LnFun>();
	Register<PowFun>();
	Register<RandomFun>();
	Register<SetseedFun>();
	Register<SqrtFun>();

	Register<PiFun>();

	Register<BitCountFun>();

	Register<GammaFun>();
	Register<LogGammaFun>();

	Register<FactorialFun>();
}

} // namespace guinsoodb

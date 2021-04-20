#include "guinsoodb/function/scalar/trigonometric_functions.hpp"
#include "guinsoodb/common/vector_operations/vector_operations.hpp"
#include "guinsoodb/common/exception.hpp"

namespace guinsoodb {

void BuiltinFunctions::RegisterTrigonometricsFunctions() {
	Register<SinFun>();
	Register<CosFun>();
	Register<TanFun>();
	Register<AsinFun>();
	Register<AcosFun>();
	Register<AtanFun>();
	Register<CotFun>();
	Register<Atan2Fun>();
}

} // namespace guinsoodb

#include "guinsoodb/function/scalar/generic_functions.hpp"

namespace guinsoodb {

void BuiltinFunctions::RegisterGenericFunctions() {
	Register<AliasFun>();
	Register<LeastFun>();
	Register<GreatestFun>();
	Register<StatsFun>();
	Register<TypeOfFun>();
	Register<CurrentSettingFun>();
}

} // namespace guinsoodb

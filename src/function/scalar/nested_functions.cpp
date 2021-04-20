#include "guinsoodb/function/scalar/nested_functions.hpp"

namespace guinsoodb {

void BuiltinFunctions::RegisterNestedFunctions() {
	Register<StructPackFun>();
	Register<StructExtractFun>();
	Register<ListValueFun>();
	Register<ListExtractFun>();
}

} // namespace guinsoodb

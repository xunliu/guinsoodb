#include "guinsoodb/function/scalar/sequence_functions.hpp"

namespace guinsoodb {

void BuiltinFunctions::RegisterSequenceFunctions() {
	Register<NextvalFun>();
}

} // namespace guinsoodb

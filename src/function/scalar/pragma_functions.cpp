#include "guinsoodb/function/pragma/pragma_functions.hpp"

namespace guinsoodb {

void BuiltinFunctions::RegisterPragmaFunctions() {
	Register<PragmaQueries>();
	Register<PragmaFunctions>();
}

} // namespace guinsoodb

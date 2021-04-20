//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/function/pragma/pragma_functions.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/function/pragma_function.hpp"

namespace guinsoodb {

struct PragmaQueries {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct PragmaFunctions {
	static void RegisterFunction(BuiltinFunctions &set);
};

} // namespace guinsoodb

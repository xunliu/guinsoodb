//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/function/scalar/sequence_functions.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/function/scalar_function.hpp"
#include "guinsoodb/function/function_set.hpp"

namespace guinsoodb {

struct NextvalFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

} // namespace guinsoodb

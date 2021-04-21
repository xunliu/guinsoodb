//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/function/scalar/blob_functions.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/function/function_set.hpp"

namespace guinsoodb {

struct Base64Fun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct EncodeFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

} // namespace guinsoodb

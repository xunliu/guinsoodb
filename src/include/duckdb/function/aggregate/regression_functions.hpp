//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/function/aggregate/regression_functions.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/function/aggregate_function.hpp"
#include "guinsoodb/function/function_set.hpp"
#include "guinsoodb/common/types/null_value.hpp"

namespace guinsoodb {
struct RegrAvgxFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct RegrAvgyFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct RegrCountFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct RegrSlopeFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct RegrR2Fun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct RegrSXXFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct RegrSYYFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct RegrSXYFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct RegrInterceptFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

} // namespace guinsoodb

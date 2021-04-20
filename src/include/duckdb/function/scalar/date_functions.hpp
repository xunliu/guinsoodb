//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/function/scalar/date_functions.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/function/scalar_function.hpp"
#include "guinsoodb/function/function_set.hpp"

namespace guinsoodb {

struct AgeFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct DatePartFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct DateTruncFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct CurrentTimeFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct CurrentDateFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct CurrentTimestampFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct EpochFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct StrfTimeFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct StrpTimeFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct ToIntervalFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

} // namespace guinsoodb

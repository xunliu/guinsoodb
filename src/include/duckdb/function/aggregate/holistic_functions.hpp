//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/function/aggregate/holistic_functions.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/function/aggregate_function.hpp"
#include "guinsoodb/function/function_set.hpp"

namespace guinsoodb {

struct QuantileFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct ApproximateQuantileFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct ReservoirQuantileFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

} // namespace guinsoodb

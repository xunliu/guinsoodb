//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/function/scalar/generic_functions.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/function/scalar_function.hpp"
#include "guinsoodb/function/function_set.hpp"

namespace guinsoodb {
class BoundFunctionExpression;

struct AliasFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct LeastFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct GreatestFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct StatsFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct TypeOfFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct ConstantOrNull {
	static ScalarFunction GetFunction(LogicalType return_type);
	static unique_ptr<FunctionData> Bind(Value value);
	static bool IsConstantOrNull(BoundFunctionExpression &expr, const Value &val);
};

struct CurrentSettingFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

} // namespace guinsoodb

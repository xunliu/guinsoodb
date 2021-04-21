//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/function/table/range.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/function/table_function.hpp"

namespace guinsoodb {

struct CheckpointFunction {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct GlobTableFunction {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct RangeTableFunction {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct RepeatTableFunction {
	static void RegisterFunction(BuiltinFunctions &set);
};

} // namespace guinsoodb

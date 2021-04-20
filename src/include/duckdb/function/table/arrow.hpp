//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/function/table/arrow.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/function/table_function.hpp"

namespace guinsoodb {

struct ArrowTableFunction {
	static void RegisterFunction(BuiltinFunctions &set);
};

} // namespace guinsoodb

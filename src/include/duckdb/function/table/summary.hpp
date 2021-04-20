//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/function/table/summary.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/function/table_function.hpp"

namespace guinsoodb {

struct SummaryTableFunction {
	static void RegisterFunction(BuiltinFunctions &set);
};

} // namespace guinsoodb

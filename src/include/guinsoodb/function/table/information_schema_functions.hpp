//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/function/table/information_schema_functions.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/function/table_function.hpp"

namespace guinsoodb {

struct InformationSchemaSchemata {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct InformationSchemaTables {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct InformationSchemaColumns {
	static void RegisterFunction(BuiltinFunctions &set);
};

} // namespace guinsoodb

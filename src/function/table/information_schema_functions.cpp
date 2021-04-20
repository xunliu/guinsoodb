#include "guinsoodb/function/table/information_schema_functions.hpp"

namespace guinsoodb {

void BuiltinFunctions::RegisterInformationSchemaFunctions() {
	InformationSchemaSchemata::RegisterFunction(*this);
	InformationSchemaTables::RegisterFunction(*this);
	InformationSchemaColumns::RegisterFunction(*this);
}

} // namespace guinsoodb

#define DUCKDB_BUILD_LOADABLE_EXTENSION
#include "guinsoodb.hpp"

using namespace guinsoodb;

inline string_t hello_fun(string_t what) {
	return "Hello, " + what.GetString();
}

extern "C" {
DUCKDB_EXTENSION_API void loadable_extension_demo_init(guinsoodb::DatabaseInstance &db) {
	Connection con(db);
	con.BeginTransaction();
	con.CreateScalarFunction<string_t, string_t>("hello", {LogicalType(LogicalTypeId::VARCHAR)},
	                                             LogicalType(LogicalTypeId::VARCHAR), &hello_fun);
	con.Commit();
}

DUCKDB_EXTENSION_API const char *loadable_extension_demo_version() {
	return GuinsooDB::LibraryVersion();
}
}

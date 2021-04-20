#include "guinsoodb/parser/statement/pragma_statement.hpp"
#include "guinsoodb/parser/transformer.hpp"

namespace guinsoodb {

unique_ptr<PragmaStatement> Transformer::TransformShow(guinsoodb_libpgquery::PGNode *node) {
	// we transform SHOW x into PRAGMA SHOW('x')

	auto stmt = reinterpret_cast<guinsoodb_libpgquery::PGVariableShowStmt *>(node);

	auto result = make_unique<PragmaStatement>();
	auto &info = *result->info;

	if (string(stmt->name) == "tables") {
		// show all tables
		info.name = "show_tables";
	} else {
		// show one specific table
		info.name = "show";
		info.parameters.emplace_back(stmt->name);
	}

	return result;
}

} // namespace guinsoodb

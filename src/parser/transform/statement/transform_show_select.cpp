#include "guinsoodb/parser/statement/show_statement.hpp"
#include "guinsoodb/parser/sql_statement.hpp"
#include "guinsoodb/parser/transformer.hpp"

namespace guinsoodb {

unique_ptr<ShowStatement> Transformer::TransformShowSelect(guinsoodb_libpgquery::PGNode *node) {
	// we capture the select statement of SHOW
	auto stmt = reinterpret_cast<guinsoodb_libpgquery::PGVariableShowSelectStmt *>(node);
	auto select_stmt = reinterpret_cast<guinsoodb_libpgquery::PGSelectStmt *>(stmt->stmt);

	auto result = make_unique<ShowStatement>();
	auto &info = *result->info;

	info.query = TransformSelectNode(select_stmt);

	return result;
}

} // namespace guinsoodb

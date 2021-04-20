#include "guinsoodb/parser/statement/select_statement.hpp"
#include "guinsoodb/parser/transformer.hpp"
#include "guinsoodb/common/string_util.hpp"

namespace guinsoodb {

unique_ptr<SelectStatement> Transformer::TransformSelect(guinsoodb_libpgquery::PGNode *node, bool is_select) {
	auto stmt = reinterpret_cast<guinsoodb_libpgquery::PGSelectStmt *>(node);
	auto result = make_unique<SelectStatement>();

	// Both Insert/Create Table As uses this.
	if (is_select) {
		if (stmt->intoClause) {
			throw ParserException("SELECT INTO not supported!");
		}
		if (stmt->lockingClause) {
			throw ParserException("SELECT locking clause is not supported!");
		}
	}

	result->node = TransformSelectNode(stmt);
	return result;
}

} // namespace guinsoodb

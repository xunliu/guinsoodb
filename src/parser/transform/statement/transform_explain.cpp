#include "guinsoodb/parser/statement/explain_statement.hpp"
#include "guinsoodb/parser/transformer.hpp"

namespace guinsoodb {

unique_ptr<ExplainStatement> Transformer::TransformExplain(guinsoodb_libpgquery::PGNode *node) {
	auto stmt = reinterpret_cast<guinsoodb_libpgquery::PGExplainStmt *>(node);
	D_ASSERT(stmt);
	return make_unique<ExplainStatement>(TransformStatement(stmt->query));
}

} // namespace guinsoodb

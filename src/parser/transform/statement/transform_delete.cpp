#include "guinsoodb/parser/statement/delete_statement.hpp"
#include "guinsoodb/parser/transformer.hpp"

namespace guinsoodb {

unique_ptr<DeleteStatement> Transformer::TransformDelete(guinsoodb_libpgquery::PGNode *node) {
	auto stmt = reinterpret_cast<guinsoodb_libpgquery::PGDeleteStmt *>(node);
	D_ASSERT(stmt);
	auto result = make_unique<DeleteStatement>();

	result->condition = TransformExpression(stmt->whereClause);
	result->table = TransformRangeVar(stmt->relation);
	if (result->table->type != TableReferenceType::BASE_TABLE) {
		throw Exception("Can only delete from base tables!");
	}
	return result;
}

} // namespace guinsoodb

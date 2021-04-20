#include "guinsoodb/parser/statement/update_statement.hpp"
#include "guinsoodb/parser/transformer.hpp"

namespace guinsoodb {

unique_ptr<UpdateStatement> Transformer::TransformUpdate(guinsoodb_libpgquery::PGNode *node) {
	auto stmt = reinterpret_cast<guinsoodb_libpgquery::PGUpdateStmt *>(node);
	D_ASSERT(stmt);

	auto result = make_unique<UpdateStatement>();

	result->table = TransformRangeVar(stmt->relation);
	if (stmt->fromClause) {
		result->from_table = TransformFrom(stmt->fromClause);
	}

	auto root = stmt->targetList;
	for (auto cell = root->head; cell != nullptr; cell = cell->next) {
		auto target = (guinsoodb_libpgquery::PGResTarget *)(cell->data.ptr_value);
		result->columns.emplace_back(target->name);
		result->expressions.push_back(TransformExpression(target->val));
	}

	result->condition = TransformExpression(stmt->whereClause);
	return result;
}

} // namespace guinsoodb

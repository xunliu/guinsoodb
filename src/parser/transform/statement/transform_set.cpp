#include "guinsoodb/parser/statement/set_statement.hpp"
#include "guinsoodb/parser/transformer.hpp"
#include "guinsoodb/parser/expression/constant_expression.hpp"

namespace guinsoodb {

unique_ptr<SetStatement> Transformer::TransformSet(guinsoodb_libpgquery::PGNode *node) {
	D_ASSERT(node->type == guinsoodb_libpgquery::T_PGVariableSetStmt);
	auto stmt = reinterpret_cast<guinsoodb_libpgquery::PGVariableSetStmt *>(node);

	if (stmt->kind != guinsoodb_libpgquery::VariableSetKind::VAR_SET_VALUE) {
		throw ParserException("Can only SET a variable to a value");
	}
	auto name = std::string(stmt->name);
	D_ASSERT(!name.empty()); // parser protect us!
	if (stmt->args->length != 1) {
		throw ParserException("SET needs a single scalar value parameter");
	}
	D_ASSERT(stmt->args->head && stmt->args->head->data.ptr_value);
	D_ASSERT(((guinsoodb_libpgquery::PGNode *)stmt->args->head->data.ptr_value)->type == guinsoodb_libpgquery::T_PGAConst);

	auto value = TransformValue(((guinsoodb_libpgquery::PGAConst *)stmt->args->head->data.ptr_value)->val)->value;

	return make_unique<SetStatement>(name, value);
}

} // namespace guinsoodb

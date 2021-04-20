#include "guinsoodb/parser/transformer.hpp"
#include "guinsoodb/parser/expression/function_expression.hpp"
#include "guinsoodb/parser/statement/call_statement.hpp"

namespace guinsoodb {

unique_ptr<SQLStatement> Transformer::TransformCheckpoint(guinsoodb_libpgquery::PGNode *node) {
	auto checkpoint = (guinsoodb_libpgquery::PGCheckPointStmt *)node;

	vector<unique_ptr<ParsedExpression>> children;
	// transform into "CALL checkpoint()" or "CALL force_checkpoint()"
	auto result = make_unique<CallStatement>();
	result->function = make_unique<FunctionExpression>(checkpoint->force ? "force_checkpoint" : "checkpoint", children);
	return move(result);
}

} // namespace guinsoodb

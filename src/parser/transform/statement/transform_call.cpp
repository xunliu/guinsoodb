#include "guinsoodb/parser/statement/call_statement.hpp"
#include "guinsoodb/parser/transformer.hpp"

namespace guinsoodb {

unique_ptr<CallStatement> Transformer::TransformCall(guinsoodb_libpgquery::PGNode *node) {
	auto stmt = reinterpret_cast<guinsoodb_libpgquery::PGCallStmt *>(node);
	D_ASSERT(stmt);

	auto result = make_unique<CallStatement>();
	result->function = TransformFuncCall((guinsoodb_libpgquery::PGFuncCall *)stmt->func);
	return result;
}

} // namespace guinsoodb

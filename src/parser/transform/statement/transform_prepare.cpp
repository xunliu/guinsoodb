#include "guinsoodb/parser/statement/drop_statement.hpp"
#include "guinsoodb/parser/statement/execute_statement.hpp"
#include "guinsoodb/parser/statement/prepare_statement.hpp"
#include "guinsoodb/parser/transformer.hpp"

namespace guinsoodb {

unique_ptr<PrepareStatement> Transformer::TransformPrepare(guinsoodb_libpgquery::PGNode *node) {
	auto stmt = reinterpret_cast<guinsoodb_libpgquery::PGPrepareStmt *>(node);
	D_ASSERT(stmt);

	if (stmt->argtypes && stmt->argtypes->length > 0) {
		throw NotImplementedException("Prepared statement argument types are not supported, use CAST");
	}

	auto result = make_unique<PrepareStatement>();
	result->name = string(stmt->name);
	result->statement = TransformStatement(stmt->query);
	SetParamCount(0);

	return result;
}

unique_ptr<ExecuteStatement> Transformer::TransformExecute(guinsoodb_libpgquery::PGNode *node) {
	auto stmt = reinterpret_cast<guinsoodb_libpgquery::PGExecuteStmt *>(node);
	D_ASSERT(stmt);

	auto result = make_unique<ExecuteStatement>();
	result->name = string(stmt->name);

	TransformExpressionList(stmt->params, result->values);
	for (auto &expr : result->values) {
		if (!expr->IsScalar()) {
			throw Exception("Only scalar parameters or NULL supported for EXECUTE");
		}
	}
	return result;
}

unique_ptr<DropStatement> Transformer::TransformDeallocate(guinsoodb_libpgquery::PGNode *node) {
	auto stmt = reinterpret_cast<guinsoodb_libpgquery::PGDeallocateStmt *>(node);
	D_ASSERT(stmt);

	auto result = make_unique<DropStatement>();
	result->info->type = CatalogType::PREPARED_STATEMENT;
	result->info->name = string(stmt->name);
	return result;
}

} // namespace guinsoodb

#include "guinsoodb/parser/statement/transaction_statement.hpp"
#include "guinsoodb/parser/transformer.hpp"

namespace guinsoodb {

unique_ptr<TransactionStatement> Transformer::TransformTransaction(guinsoodb_libpgquery::PGNode *node) {
	auto stmt = reinterpret_cast<guinsoodb_libpgquery::PGTransactionStmt *>(node);
	D_ASSERT(stmt);
	switch (stmt->kind) {
	case guinsoodb_libpgquery::PG_TRANS_STMT_BEGIN:
	case guinsoodb_libpgquery::PG_TRANS_STMT_START:
		return make_unique<TransactionStatement>(TransactionType::BEGIN_TRANSACTION);
	case guinsoodb_libpgquery::PG_TRANS_STMT_COMMIT:
		return make_unique<TransactionStatement>(TransactionType::COMMIT);
	case guinsoodb_libpgquery::PG_TRANS_STMT_ROLLBACK:
		return make_unique<TransactionStatement>(TransactionType::ROLLBACK);
	default:
		throw NotImplementedException("Transaction type %d not implemented yet", stmt->kind);
	}
}

} // namespace guinsoodb

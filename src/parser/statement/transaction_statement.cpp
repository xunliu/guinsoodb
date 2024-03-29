#include "guinsoodb/parser/statement/transaction_statement.hpp"

namespace guinsoodb {

TransactionStatement::TransactionStatement(TransactionType type)
    : SQLStatement(StatementType::TRANSACTION_STATEMENT), info(make_unique<TransactionInfo>(type)) {
}

unique_ptr<SQLStatement> TransactionStatement::Copy() const {
	return make_unique_base<SQLStatement, TransactionStatement>(info->type);
}

} // namespace guinsoodb

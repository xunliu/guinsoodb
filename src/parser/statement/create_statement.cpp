#include "guinsoodb/parser/statement/create_statement.hpp"

namespace guinsoodb {

CreateStatement::CreateStatement() : SQLStatement(StatementType::CREATE_STATEMENT) {
}

unique_ptr<SQLStatement> CreateStatement::Copy() const {
	auto result = make_unique<CreateStatement>();
	result->info = info->Copy();
	return move(result);
}

} // namespace guinsoodb

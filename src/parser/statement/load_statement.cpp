#include "guinsoodb/parser/statement/load_statement.hpp"

namespace guinsoodb {

LoadStatement::LoadStatement() : SQLStatement(StatementType::LOAD_STATEMENT) {
}

unique_ptr<SQLStatement> LoadStatement::Copy() const {
	auto result = make_unique<LoadStatement>();
	result->info = info->Copy();
	return move(result);
}

} // namespace guinsoodb

#include "guinsoodb/parser/statement/pragma_statement.hpp"

namespace guinsoodb {

PragmaStatement::PragmaStatement() : SQLStatement(StatementType::PRAGMA_STATEMENT), info(make_unique<PragmaInfo>()) {
}

unique_ptr<SQLStatement> PragmaStatement::Copy() const {
	auto result = make_unique<PragmaStatement>();
	result->info = info->Copy();
	return move(result);
}

} // namespace guinsoodb

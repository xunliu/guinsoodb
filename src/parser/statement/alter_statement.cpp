#include "guinsoodb/parser/statement/alter_statement.hpp"

namespace guinsoodb {

AlterStatement::AlterStatement() : SQLStatement(StatementType::ALTER_STATEMENT) {
}
AlterStatement::AlterStatement(unique_ptr<AlterInfo> info)
    : SQLStatement(StatementType::ALTER_STATEMENT), info(move(info)) {
}

unique_ptr<SQLStatement> AlterStatement::Copy() const {
	auto result = make_unique<AlterStatement>(info->Copy());
	return move(result);
}

} // namespace guinsoodb

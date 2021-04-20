#include "guinsoodb/parser/statement/delete_statement.hpp"

namespace guinsoodb {

DeleteStatement::DeleteStatement() : SQLStatement(StatementType::DELETE_STATEMENT) {
}

unique_ptr<SQLStatement> DeleteStatement::Copy() const {
	auto result = make_unique<DeleteStatement>();
	if (condition) {
		result->condition = condition->Copy();
	}
	result->table = table->Copy();
	return move(result);
}

} // namespace guinsoodb

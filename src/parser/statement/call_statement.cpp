#include "guinsoodb/parser/statement/call_statement.hpp"

namespace guinsoodb {

CallStatement::CallStatement() : SQLStatement(StatementType::CALL_STATEMENT) {
}

unique_ptr<SQLStatement> CallStatement::Copy() const {
	auto result = make_unique<CallStatement>();
	result->function = function->Copy();
	return move(result);
}

} // namespace guinsoodb

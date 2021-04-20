#include "guinsoodb/parser/statement/execute_statement.hpp"

namespace guinsoodb {

ExecuteStatement::ExecuteStatement() : SQLStatement(StatementType::EXECUTE_STATEMENT) {
}

unique_ptr<SQLStatement> ExecuteStatement::Copy() const {
	auto result = make_unique<ExecuteStatement>();
	result->name = name;
	for (auto &value : values) {
		result->values.push_back(value->Copy());
	}
	return move(result);
}

} // namespace guinsoodb

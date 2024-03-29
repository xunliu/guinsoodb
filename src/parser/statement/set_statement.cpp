#include "guinsoodb/parser/statement/set_statement.hpp"

namespace guinsoodb {

SetStatement::SetStatement(std::string name_p, Value value_p)
    : SQLStatement(StatementType::SET_STATEMENT), name(move(name_p)), value(move(value_p)) {
}

unique_ptr<SQLStatement> SetStatement::Copy() const {
	return make_unique<SetStatement>(name, value);
}

} // namespace guinsoodb

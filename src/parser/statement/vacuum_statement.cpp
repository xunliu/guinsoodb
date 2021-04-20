#include "guinsoodb/parser/statement/vacuum_statement.hpp"

namespace guinsoodb {

VacuumStatement::VacuumStatement() : SQLStatement(StatementType::VACUUM_STATEMENT) {
}

unique_ptr<SQLStatement> VacuumStatement::Copy() const {
	return make_unique<VacuumStatement>();
}

} // namespace guinsoodb

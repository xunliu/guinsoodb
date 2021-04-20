#include "guinsoodb/planner/binder.hpp"
#include "guinsoodb/parser/statement/vacuum_statement.hpp"
#include "guinsoodb/planner/operator/logical_simple.hpp"

namespace guinsoodb {

BoundStatement Binder::Bind(VacuumStatement &stmt) {
	BoundStatement result;
	result.names = {"Success"};
	result.types = {LogicalType::BOOLEAN};
	result.plan = make_unique<LogicalSimple>(LogicalOperatorType::LOGICAL_VACUUM, move(stmt.info));
	return result;
}

} // namespace guinsoodb

#include "guinsoodb/parser/statement/load_statement.hpp"
#include "guinsoodb/planner/binder.hpp"
#include "guinsoodb/planner/operator/logical_simple.hpp"
#include <algorithm>

namespace guinsoodb {

BoundStatement Binder::Bind(LoadStatement &stmt) {
	BoundStatement result;
	result.types = {LogicalType::BOOLEAN};
	result.names = {"Success"};

	result.plan = make_unique<LogicalSimple>(LogicalOperatorType::LOGICAL_LOAD, move(stmt.info));
	return result;
}

} // namespace guinsoodb

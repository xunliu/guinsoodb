#include "guinsoodb/parser/statement/set_statement.hpp"
#include "guinsoodb/planner/binder.hpp"
#include "guinsoodb/planner/operator/logical_set.hpp"
#include <algorithm>

namespace guinsoodb {

BoundStatement Binder::Bind(SetStatement &stmt) {
	BoundStatement result;
	result.types = {LogicalType::BOOLEAN};
	result.names = {"Success"};

	result.plan = make_unique<LogicalSet>(stmt.name, stmt.value);
	return result;
}

} // namespace guinsoodb

#include "guinsoodb/parser/statement/select_statement.hpp"
#include "guinsoodb/planner/binder.hpp"
#include "guinsoodb/planner/bound_query_node.hpp"

namespace guinsoodb {

BoundStatement Binder::Bind(SelectStatement &stmt) {
	this->allow_stream_result = true;
	return Bind(*stmt.node);
}

} // namespace guinsoodb

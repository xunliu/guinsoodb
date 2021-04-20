#include "guinsoodb/catalog/catalog.hpp"
#include "guinsoodb/parser/expression/constant_expression.hpp"
#include "guinsoodb/parser/statement/insert_statement.hpp"
#include "guinsoodb/parser/query_node/select_node.hpp"
#include "guinsoodb/planner/binder.hpp"
#include "guinsoodb/parser/statement/relation_statement.hpp"

namespace guinsoodb {

BoundStatement Binder::Bind(RelationStatement &stmt) {
	return stmt.relation->Bind(*this);
}

} // namespace guinsoodb

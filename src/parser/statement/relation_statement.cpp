#include "guinsoodb/parser/statement/relation_statement.hpp"

namespace guinsoodb {

RelationStatement::RelationStatement(shared_ptr<Relation> relation)
    : SQLStatement(StatementType::RELATION_STATEMENT), relation(move(relation)) {
}

unique_ptr<SQLStatement> RelationStatement::Copy() const {
	auto result = make_unique<RelationStatement>(relation);
	return move(result);
}

} // namespace guinsoodb

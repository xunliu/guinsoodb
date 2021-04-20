//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/statement/relation_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/sql_statement.hpp"
#include "guinsoodb/main/relation.hpp"

namespace guinsoodb {

class RelationStatement : public SQLStatement {
public:
	explicit RelationStatement(shared_ptr<Relation> relation);

	shared_ptr<Relation> relation;

public:
	unique_ptr<SQLStatement> Copy() const override;
};

} // namespace guinsoodb

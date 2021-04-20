//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/main/relation/projection_relation.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/main/relation.hpp"
#include "guinsoodb/parser/parsed_expression.hpp"

namespace guinsoodb {

class ProjectionRelation : public Relation {
public:
	ProjectionRelation(shared_ptr<Relation> child, vector<unique_ptr<ParsedExpression>> expressions,
	                   vector<string> aliases);

	vector<unique_ptr<ParsedExpression>> expressions;
	vector<ColumnDefinition> columns;
	shared_ptr<Relation> child;

public:
	unique_ptr<QueryNode> GetQueryNode() override;

	const vector<ColumnDefinition> &Columns() override;
	string ToString(idx_t depth) override;
	string GetAlias() override;
};

} // namespace guinsoodb

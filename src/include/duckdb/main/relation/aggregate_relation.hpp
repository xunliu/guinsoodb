//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/main/relation/aggregate_relation.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/main/relation.hpp"
#include "guinsoodb/parser/parsed_expression.hpp"

namespace guinsoodb {

class AggregateRelation : public Relation {
public:
	AggregateRelation(shared_ptr<Relation> child, vector<unique_ptr<ParsedExpression>> expressions);
	AggregateRelation(shared_ptr<Relation> child, vector<unique_ptr<ParsedExpression>> expressions,
	                  vector<unique_ptr<ParsedExpression>> groups);

	vector<unique_ptr<ParsedExpression>> expressions;
	vector<unique_ptr<ParsedExpression>> groups;
	vector<ColumnDefinition> columns;
	shared_ptr<Relation> child;

public:
	unique_ptr<QueryNode> GetQueryNode() override;

	const vector<ColumnDefinition> &Columns() override;
	string ToString(idx_t depth) override;
	string GetAlias() override;
};

} // namespace guinsoodb

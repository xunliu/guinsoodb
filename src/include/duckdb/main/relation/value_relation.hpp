//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/main/relation/value_relation.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/main/relation.hpp"
#include "guinsoodb/parser/parsed_expression.hpp"

namespace guinsoodb {

class ValueRelation : public Relation {
public:
	ValueRelation(ClientContext &context, const vector<vector<Value>> &values, vector<string> names,
	              string alias = "values");
	ValueRelation(ClientContext &context, const string &values, vector<string> names, string alias = "values");

	vector<vector<unique_ptr<ParsedExpression>>> expressions;
	vector<string> names;
	vector<ColumnDefinition> columns;
	string alias;

public:
	unique_ptr<QueryNode> GetQueryNode() override;

	const vector<ColumnDefinition> &Columns() override;
	string ToString(idx_t depth) override;
	string GetAlias() override;

	unique_ptr<TableRef> GetTableRef() override;
};

} // namespace guinsoodb

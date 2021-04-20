//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/main/relation/update_relation.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/main/relation.hpp"
#include "guinsoodb/parser/parsed_expression.hpp"

namespace guinsoodb {

class UpdateRelation : public Relation {
public:
	UpdateRelation(ClientContext &context, unique_ptr<ParsedExpression> condition, string schema_name,
	               string table_name, vector<string> update_columns, vector<unique_ptr<ParsedExpression>> expressions);

	vector<ColumnDefinition> columns;
	unique_ptr<ParsedExpression> condition;
	string schema_name;
	string table_name;
	vector<string> update_columns;
	vector<unique_ptr<ParsedExpression>> expressions;

public:
	unique_ptr<QueryNode> GetQueryNode() override;
	BoundStatement Bind(Binder &binder) override;
	const vector<ColumnDefinition> &Columns() override;
	string ToString(idx_t depth) override;
	bool IsReadOnly() override {
		return false;
	}
};

} // namespace guinsoodb

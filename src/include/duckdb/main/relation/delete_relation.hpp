//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/main/relation/delete_relation.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/main/relation.hpp"
#include "guinsoodb/parser/parsed_expression.hpp"

namespace guinsoodb {

class DeleteRelation : public Relation {
public:
	DeleteRelation(ClientContext &context, unique_ptr<ParsedExpression> condition, string schema_name,
	               string table_name);

	vector<ColumnDefinition> columns;
	unique_ptr<ParsedExpression> condition;
	string schema_name;
	string table_name;

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

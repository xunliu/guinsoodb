//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/main/relation/create_table_relation.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/main/relation.hpp"

namespace guinsoodb {

class CreateTableRelation : public Relation {
public:
	CreateTableRelation(shared_ptr<Relation> child, string schema_name, string table_name);

	shared_ptr<Relation> child;
	string schema_name;
	string table_name;
	vector<ColumnDefinition> columns;

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

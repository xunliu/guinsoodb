//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/main/relation/view_relation.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/main/relation.hpp"

namespace guinsoodb {

class ViewRelation : public Relation {
public:
	ViewRelation(ClientContext &context, string schema_name, string view_name);

	string schema_name;
	string view_name;
	vector<ColumnDefinition> columns;

public:
	unique_ptr<QueryNode> GetQueryNode() override;
	unique_ptr<TableRef> GetTableRef() override;

	const vector<ColumnDefinition> &Columns() override;
	string ToString(idx_t depth) override;
	string GetAlias() override;
};

} // namespace guinsoodb

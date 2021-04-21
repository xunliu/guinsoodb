//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/main/relation/table_relation.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/main/relation.hpp"
#include "guinsoodb/main/table_description.hpp"

namespace guinsoodb {

class TableRelation : public Relation {
public:
	TableRelation(ClientContext &context, unique_ptr<TableDescription> description);

	unique_ptr<TableDescription> description;

public:
	unique_ptr<QueryNode> GetQueryNode() override;

	const vector<ColumnDefinition> &Columns() override;
	string ToString(idx_t depth) override;
	string GetAlias() override;

	unique_ptr<TableRef> GetTableRef() override;

	void Update(const string &update, const string &condition = string()) override;
	void Delete(const string &condition = string()) override;
};

} // namespace guinsoodb

//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/main/relation/create_view_relation.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/main/relation.hpp"

namespace guinsoodb {

class CreateViewRelation : public Relation {
public:
	CreateViewRelation(shared_ptr<Relation> child, string view_name, bool replace, bool temporary);

	shared_ptr<Relation> child;
	string view_name;
	bool replace;
	bool temporary;
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

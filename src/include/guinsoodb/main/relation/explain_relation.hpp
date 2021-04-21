//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/main/relation/explain_relation.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/main/relation.hpp"

namespace guinsoodb {

class ExplainRelation : public Relation {
public:
	explicit ExplainRelation(shared_ptr<Relation> child);

	shared_ptr<Relation> child;
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

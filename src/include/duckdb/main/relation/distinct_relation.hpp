//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/main/relation/distinct_relation.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/main/relation.hpp"

namespace guinsoodb {

class DistinctRelation : public Relation {
public:
	explicit DistinctRelation(shared_ptr<Relation> child);

	shared_ptr<Relation> child;

public:
	unique_ptr<QueryNode> GetQueryNode() override;

	const vector<ColumnDefinition> &Columns() override;
	string ToString(idx_t depth) override;
	string GetAlias() override;

public:
	bool InheritsColumnBindings() override {
		return true;
	}
	Relation *ChildRelation() override {
		return child.get();
	}
};

} // namespace guinsoodb

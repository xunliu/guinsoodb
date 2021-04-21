//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/main/relation/order_relation.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/main/relation.hpp"
#include "guinsoodb/parser/parsed_expression.hpp"
#include "guinsoodb/parser/result_modifier.hpp"

namespace guinsoodb {

class OrderRelation : public Relation {
public:
	OrderRelation(shared_ptr<Relation> child, vector<OrderByNode> orders);

	vector<OrderByNode> orders;
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

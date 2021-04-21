//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/main/relation/setop_relation.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/main/relation.hpp"
#include "guinsoodb/common/enums/set_operation_type.hpp"

namespace guinsoodb {

class SetOpRelation : public Relation {
public:
	SetOpRelation(shared_ptr<Relation> left, shared_ptr<Relation> right, SetOperationType setop_type);

	shared_ptr<Relation> left;
	shared_ptr<Relation> right;
	SetOperationType setop_type;

public:
	unique_ptr<QueryNode> GetQueryNode() override;

	const vector<ColumnDefinition> &Columns() override;
	string ToString(idx_t depth) override;
	string GetAlias() override;
};

} // namespace guinsoodb

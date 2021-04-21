//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/tableref/bound_joinref.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/planner/binder.hpp"
#include "guinsoodb/common/enums/join_type.hpp"
#include "guinsoodb/planner/bound_tableref.hpp"
#include "guinsoodb/planner/expression.hpp"

namespace guinsoodb {

//! Represents a join
class BoundJoinRef : public BoundTableRef {
public:
	BoundJoinRef() : BoundTableRef(TableReferenceType::JOIN) {
	}

	//! The binder used to bind the LHS of the join
	shared_ptr<Binder> left_binder;
	//! The binder used to bind the RHS of the join
	shared_ptr<Binder> right_binder;
	//! The left hand side of the join
	unique_ptr<BoundTableRef> left;
	//! The right hand side of the join
	unique_ptr<BoundTableRef> right;
	//! The join condition
	unique_ptr<Expression> condition;
	//! The join type
	JoinType type;
};
} // namespace guinsoodb

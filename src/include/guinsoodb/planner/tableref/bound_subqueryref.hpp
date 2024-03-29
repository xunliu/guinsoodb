//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/tableref/bound_subqueryref.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/planner/binder.hpp"
#include "guinsoodb/planner/bound_query_node.hpp"
#include "guinsoodb/planner/bound_tableref.hpp"

namespace guinsoodb {

//! Represents a cross product
class BoundSubqueryRef : public BoundTableRef {
public:
	BoundSubqueryRef(shared_ptr<Binder> binder_p, unique_ptr<BoundQueryNode> subquery)
	    : BoundTableRef(TableReferenceType::SUBQUERY), binder(move(binder_p)), subquery(move(subquery)) {
	}

	//! The binder used to bind the subquery
	shared_ptr<Binder> binder;
	//! The bound subquery node
	unique_ptr<BoundQueryNode> subquery;
};
} // namespace guinsoodb

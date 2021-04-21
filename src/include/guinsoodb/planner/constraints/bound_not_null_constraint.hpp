//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/constraints/bound_not_null_constraint.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/planner/bound_constraint.hpp"

namespace guinsoodb {

class BoundNotNullConstraint : public BoundConstraint {
public:
	explicit BoundNotNullConstraint(column_t index) : BoundConstraint(ConstraintType::NOT_NULL), index(index) {
	}

	//! Column index this constraint pertains to
	column_t index;
};

} // namespace guinsoodb

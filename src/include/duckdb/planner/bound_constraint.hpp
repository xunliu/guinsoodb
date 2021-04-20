//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/bound_constraint.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/common.hpp"
#include "guinsoodb/parser/constraint.hpp"

namespace guinsoodb {
//! Bound equivalent of Constraint
class BoundConstraint {
public:
	explicit BoundConstraint(ConstraintType type) : type(type) {};
	virtual ~BoundConstraint() {
	}

	ConstraintType type;
};
} // namespace guinsoodb

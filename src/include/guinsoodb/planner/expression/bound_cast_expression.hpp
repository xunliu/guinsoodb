//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/expression/bound_cast_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/planner/expression.hpp"

namespace guinsoodb {

class BoundCastExpression : public Expression {
public:
	BoundCastExpression(unique_ptr<Expression> child, LogicalType target_type);

	//! The child type
	unique_ptr<Expression> child;

public:
	LogicalType source_type() {
		return child->return_type;
	}

	//! Cast an expression to the specified SQL type if required
	static unique_ptr<Expression> AddCastToType(unique_ptr<Expression> expr, const LogicalType &target_type);
	//! Returns true if a cast is invertible (i.e. CAST(s -> t -> s) = s for all values of s). This is not true for e.g.
	//! boolean casts, because that can be e.g. -1 -> TRUE -> 1. This is necessary to prevent some optimizer bugs.
	static bool CastIsInvertible(const LogicalType &source_type, const LogicalType &target_type);

	string ToString() const override;

	bool Equals(const BaseExpression *other) const override;

	unique_ptr<Expression> Copy() override;
};
} // namespace guinsoodb

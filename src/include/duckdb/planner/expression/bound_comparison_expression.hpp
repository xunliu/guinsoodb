//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/expression/bound_comparison_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/planner/expression.hpp"

namespace guinsoodb {

class BoundComparisonExpression : public Expression {
public:
	BoundComparisonExpression(ExpressionType type, unique_ptr<Expression> left, unique_ptr<Expression> right);

	unique_ptr<Expression> left;
	unique_ptr<Expression> right;

public:
	string ToString() const override;

	bool Equals(const BaseExpression *other) const override;

	unique_ptr<Expression> Copy() override;

public:
	static LogicalType BindComparison(LogicalType left_type, LogicalType right_type);
};
} // namespace guinsoodb

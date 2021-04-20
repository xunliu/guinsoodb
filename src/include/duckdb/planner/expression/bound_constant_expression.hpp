//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/expression/bound_constant_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/types/value.hpp"
#include "guinsoodb/planner/expression.hpp"

namespace guinsoodb {

class BoundConstantExpression : public Expression {
public:
	explicit BoundConstantExpression(Value value);

	Value value;

public:
	string ToString() const override;

	bool Equals(const BaseExpression *other) const override;
	hash_t Hash() const override;

	unique_ptr<Expression> Copy() override;
};
} // namespace guinsoodb

//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/expression/bound_parameter_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/types/value.hpp"
#include "guinsoodb/planner/expression.hpp"

namespace guinsoodb {

class BoundParameterExpression : public Expression {
public:
	explicit BoundParameterExpression(idx_t parameter_nr);

	idx_t parameter_nr;
	Value *value;

public:
	bool IsScalar() const override;
	bool HasParameter() const override;
	bool IsFoldable() const override;

	string ToString() const override;

	bool Equals(const BaseExpression *other) const override;
	hash_t Hash() const override;

	unique_ptr<Expression> Copy() override;
};
} // namespace guinsoodb

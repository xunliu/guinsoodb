#include "guinsoodb/planner/expression/bound_constant_expression.hpp"

#include "guinsoodb/common/types/hash.hpp"
#include "guinsoodb/common/value_operations/value_operations.hpp"

namespace guinsoodb {

BoundConstantExpression::BoundConstantExpression(Value value_p)
    : Expression(ExpressionType::VALUE_CONSTANT, ExpressionClass::BOUND_CONSTANT, value_p.type()),
      value(move(value_p)) {
}

string BoundConstantExpression::ToString() const {
	return value.ToString();
}

bool BoundConstantExpression::Equals(const BaseExpression *other_p) const {
	if (!Expression::Equals(other_p)) {
		return false;
	}
	auto other = (BoundConstantExpression *)other_p;
	return value == other->value;
}

hash_t BoundConstantExpression::Hash() const {
	hash_t result = Expression::Hash();
	return CombineHash(ValueOperations::Hash(value), result);
}

unique_ptr<Expression> BoundConstantExpression::Copy() {
	auto copy = make_unique<BoundConstantExpression>(value);
	copy->CopyProperties(*this);
	return move(copy);
}

} // namespace guinsoodb

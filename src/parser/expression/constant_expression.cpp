#include "guinsoodb/parser/expression/constant_expression.hpp"

#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/common/types/hash.hpp"
#include "guinsoodb/common/value_operations/value_operations.hpp"

namespace guinsoodb {

ConstantExpression::ConstantExpression(Value val)
    : ParsedExpression(ExpressionType::VALUE_CONSTANT, ExpressionClass::CONSTANT), value(move(val)) {
}

string ConstantExpression::ToString() const {
	return value.ToString();
}

bool ConstantExpression::Equals(const ConstantExpression *a, const ConstantExpression *b) {
	return a->value == b->value;
}

hash_t ConstantExpression::Hash() const {
	return ParsedExpression::Hash();
}

unique_ptr<ParsedExpression> ConstantExpression::Copy() const {
	auto copy = make_unique<ConstantExpression>(value);
	copy->CopyProperties(*this);
	return move(copy);
}

void ConstantExpression::Serialize(Serializer &serializer) {
	ParsedExpression::Serialize(serializer);
	value.Serialize(serializer);
}

unique_ptr<ParsedExpression> ConstantExpression::Deserialize(ExpressionType type, Deserializer &source) {
	Value value = Value::Deserialize(source);
	return make_unique<ConstantExpression>(value);
}

} // namespace guinsoodb

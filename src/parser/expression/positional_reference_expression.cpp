#include "guinsoodb/parser/expression/positional_reference_expression.hpp"

#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/common/serializer.hpp"
#include "guinsoodb/common/types/hash.hpp"
#include "guinsoodb/common/to_string.hpp"

namespace guinsoodb {

PositionalReferenceExpression::PositionalReferenceExpression(idx_t index)
    : ParsedExpression(ExpressionType::POSITIONAL_REFERENCE, ExpressionClass::POSITIONAL_REFERENCE), index(index) {
}

string PositionalReferenceExpression::ToString() const {
	return "#" + to_string(index);
}

bool PositionalReferenceExpression::Equals(const PositionalReferenceExpression *a,
                                           const PositionalReferenceExpression *b) {
	return a->index == b->index;
}

unique_ptr<ParsedExpression> PositionalReferenceExpression::Copy() const {
	auto copy = make_unique<PositionalReferenceExpression>(index);
	copy->CopyProperties(*this);
	return move(copy);
}

hash_t PositionalReferenceExpression::Hash() const {
	hash_t result = ParsedExpression::Hash();
	return CombineHash(guinsoodb::Hash(index), result);
}

void PositionalReferenceExpression::Serialize(Serializer &serializer) {
	ParsedExpression::Serialize(serializer);
	serializer.Write<idx_t>(index);
}

unique_ptr<ParsedExpression> PositionalReferenceExpression::Deserialize(ExpressionType type, Deserializer &source) {
	auto expression = make_unique<PositionalReferenceExpression>(source.Read<idx_t>());
	return move(expression);
}

} // namespace guinsoodb

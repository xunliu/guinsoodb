//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/expression/positional_reference_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_expression.hpp"

namespace guinsoodb {
class PositionalReferenceExpression : public ParsedExpression {
public:
	PositionalReferenceExpression(idx_t index);

	idx_t index;

public:
	bool IsScalar() const override {
		return false;
	}

	string ToString() const override;

	static bool Equals(const PositionalReferenceExpression *a, const PositionalReferenceExpression *b);
	unique_ptr<ParsedExpression> Copy() const override;
	hash_t Hash() const override;

	void Serialize(Serializer &serializer) override;
	static unique_ptr<ParsedExpression> Deserialize(ExpressionType type, Deserializer &source);
};
} // namespace guinsoodb

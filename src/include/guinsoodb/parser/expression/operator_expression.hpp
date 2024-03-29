//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/expression/operator_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_expression.hpp"
#include "guinsoodb/common/vector.hpp"

namespace guinsoodb {
//! Represents a built-in operator expression
class OperatorExpression : public ParsedExpression {
public:
	explicit OperatorExpression(ExpressionType type, unique_ptr<ParsedExpression> left = nullptr,
	                            unique_ptr<ParsedExpression> right = nullptr);
	OperatorExpression(ExpressionType type, vector<unique_ptr<ParsedExpression>> children);

	vector<unique_ptr<ParsedExpression>> children;

public:
	string ToString() const override;

	static bool Equals(const OperatorExpression *a, const OperatorExpression *b);

	unique_ptr<ParsedExpression> Copy() const override;

	void Serialize(Serializer &serializer) override;
	static unique_ptr<ParsedExpression> Deserialize(ExpressionType type, Deserializer &source);
};
} // namespace guinsoodb

//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/expression/default_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_expression.hpp"

namespace guinsoodb {
//! Represents the default value of a column
class DefaultExpression : public ParsedExpression {
public:
	DefaultExpression();

public:
	bool IsScalar() const override {
		return false;
	}

	string ToString() const override;

	unique_ptr<ParsedExpression> Copy() const override;

	static unique_ptr<ParsedExpression> Deserialize(ExpressionType type, Deserializer &source);
};
} // namespace guinsoodb

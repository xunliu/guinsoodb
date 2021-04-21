//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/expression/star_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_expression.hpp"

namespace guinsoodb {

//! Represents a * expression in the SELECT clause
class StarExpression : public ParsedExpression {
public:
	StarExpression();

public:
	string ToString() const override;

	unique_ptr<ParsedExpression> Copy() const override;

	static unique_ptr<ParsedExpression> Deserialize(ExpressionType type, Deserializer &source);
};
} // namespace guinsoodb

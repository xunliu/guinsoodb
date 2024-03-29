//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/expression/case_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_expression.hpp"
#include "guinsoodb/common/vector.hpp"

namespace guinsoodb {

struct CaseCheck {
	unique_ptr<ParsedExpression> when_expr;
	unique_ptr<ParsedExpression> then_expr;
};

//! The CaseExpression represents a CASE expression in the query
class CaseExpression : public ParsedExpression {
public:
	CaseExpression();

	vector<CaseCheck> case_checks;
	unique_ptr<ParsedExpression> else_expr;

public:
	string ToString() const override;

	static bool Equals(const CaseExpression *a, const CaseExpression *b);

	unique_ptr<ParsedExpression> Copy() const override;

	void Serialize(Serializer &serializer) override;
	static unique_ptr<ParsedExpression> Deserialize(ExpressionType type, Deserializer &source);
};
} // namespace guinsoodb

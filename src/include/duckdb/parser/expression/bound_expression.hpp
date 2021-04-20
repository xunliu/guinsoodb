//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/expression/bound_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/parser/parsed_expression.hpp"
#include "guinsoodb/planner/expression.hpp"

namespace guinsoodb {

//! BoundExpression is an intermediate dummy class used by the binder. It is a ParsedExpression but holds an Expression.
//! It represents a successfully bound expression. It is used in the Binder to prevent re-binding of already bound parts
//! when dealing with subqueries.
class BoundExpression : public ParsedExpression {
public:
	BoundExpression(unique_ptr<Expression> expr, unique_ptr<ParsedExpression> parsed_expr)
	    : ParsedExpression(ExpressionType::INVALID, ExpressionClass::BOUND_EXPRESSION), expr(move(expr)),
	      parsed_expr(move(parsed_expr)) {
	}

	unique_ptr<Expression> expr;
	unique_ptr<ParsedExpression> parsed_expr;

public:
	string ToString() const override {
		return expr->ToString();
	}

	bool Equals(const BaseExpression *other) const override {
		return parsed_expr->Equals(other);
	}
	hash_t Hash() const override {
		return parsed_expr->Hash();
	}

	unique_ptr<ParsedExpression> Copy() const override {
		throw SerializationException("Cannot copy or serialize bound expression");
	}
};

} // namespace guinsoodb

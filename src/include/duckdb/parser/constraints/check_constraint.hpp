//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/constraints/check_constraint.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/string_util.hpp"
#include "guinsoodb/parser/constraint.hpp"
#include "guinsoodb/parser/parsed_expression.hpp"

namespace guinsoodb {

//! The CheckConstraint contains an expression that must evaluate to TRUE for
//! every row in a table
class CheckConstraint : public Constraint {
public:
	explicit CheckConstraint(unique_ptr<ParsedExpression> expression)
	    : Constraint(ConstraintType::CHECK), expression(move(expression)) {};

	unique_ptr<ParsedExpression> expression;

public:
	string ToString() const override;

	unique_ptr<Constraint> Copy() override;

	//! Serialize to a stand-alone binary blob
	void Serialize(Serializer &serializer) override;
	//! Deserializes a CheckConstraint
	static unique_ptr<Constraint> Deserialize(Deserializer &source);
};

} // namespace guinsoodb

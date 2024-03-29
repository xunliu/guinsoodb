//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/constraints/not_null_constraint.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/constraint.hpp"

namespace guinsoodb {

class NotNullConstraint : public Constraint {
public:
	explicit NotNullConstraint(column_t index) : Constraint(ConstraintType::NOT_NULL), index(index) {};
	~NotNullConstraint() override {
	}

	//! Column index this constraint pertains to
	column_t index;

public:
	string ToString() const override;

	unique_ptr<Constraint> Copy() override;

	//! Serialize to a stand-alone binary blob
	void Serialize(Serializer &serializer) override;
	//! Deserializes a NotNullConstraint
	static unique_ptr<Constraint> Deserialize(Deserializer &source);
};

} // namespace guinsoodb

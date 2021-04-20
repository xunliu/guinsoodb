#include "guinsoodb/parser/constraints/check_constraint.hpp"

#include "guinsoodb/common/serializer.hpp"

namespace guinsoodb {

string CheckConstraint::ToString() const {
	return "CHECK(" + expression->ToString() + ")";
}

unique_ptr<Constraint> CheckConstraint::Copy() {
	return make_unique<CheckConstraint>(expression->Copy());
}

void CheckConstraint::Serialize(Serializer &serializer) {
	Constraint::Serialize(serializer);
	expression->Serialize(serializer);
}

unique_ptr<Constraint> CheckConstraint::Deserialize(Deserializer &source) {
	auto expression = ParsedExpression::Deserialize(source);
	return make_unique<CheckConstraint>(move(expression));
}

} // namespace guinsoodb

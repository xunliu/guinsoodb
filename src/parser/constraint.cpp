#include "guinsoodb/parser/constraint.hpp"

#include "guinsoodb/common/printer.hpp"
#include "guinsoodb/common/serializer.hpp"
#include "guinsoodb/parser/constraints/list.hpp"

namespace guinsoodb {

void Constraint::Serialize(Serializer &serializer) {
	serializer.Write<ConstraintType>(type);
}

unique_ptr<Constraint> Constraint::Deserialize(Deserializer &source) {
	auto type = source.Read<ConstraintType>();
	switch (type) {
	case ConstraintType::NOT_NULL:
		return NotNullConstraint::Deserialize(source);
	case ConstraintType::CHECK:
		return CheckConstraint::Deserialize(source);
	case ConstraintType::UNIQUE:
		return UniqueConstraint::Deserialize(source);
	default:
		// don't know how to serialize this constraint type
		return nullptr;
	}
}

void Constraint::Print() {
	Printer::Print(ToString());
}

} // namespace guinsoodb

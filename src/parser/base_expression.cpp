#include "guinsoodb/parser/base_expression.hpp"

#include "guinsoodb/common/printer.hpp"

namespace guinsoodb {

void BaseExpression::Print() {
	Printer::Print(ToString());
}

string BaseExpression::GetName() const {
	return !alias.empty() ? alias : ToString();
}

bool BaseExpression::Equals(const BaseExpression *other) const {
	if (!other) {
		return false;
	}
	if (this->expression_class != other->expression_class || this->type != other->type) {
		return false;
	}
	return true;
}

} // namespace guinsoodb

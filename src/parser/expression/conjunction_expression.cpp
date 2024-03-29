#include "guinsoodb/parser/expression/conjunction_expression.hpp"
#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/common/serializer.hpp"
#include "guinsoodb/parser/expression_util.hpp"

namespace guinsoodb {

ConjunctionExpression::ConjunctionExpression(ExpressionType type)
    : ParsedExpression(type, ExpressionClass::CONJUNCTION) {
}

ConjunctionExpression::ConjunctionExpression(ExpressionType type, vector<unique_ptr<ParsedExpression>> children)
    : ParsedExpression(type, ExpressionClass::CONJUNCTION) {
	for (auto &child : children) {
		AddExpression(move(child));
	}
}

ConjunctionExpression::ConjunctionExpression(ExpressionType type, unique_ptr<ParsedExpression> left,
                                             unique_ptr<ParsedExpression> right)
    : ParsedExpression(type, ExpressionClass::CONJUNCTION) {
	AddExpression(move(left));
	AddExpression(move(right));
}

void ConjunctionExpression::AddExpression(unique_ptr<ParsedExpression> expr) {
	if (expr->type == type) {
		// expr is a conjunction of the same type: merge the expression lists together
		auto &other = (ConjunctionExpression &)*expr;
		for (auto &child : other.children) {
			children.push_back(move(child));
		}
	} else {
		children.push_back(move(expr));
	}
}

string ConjunctionExpression::ToString() const {
	string result = children[0]->ToString();
	for (idx_t i = 1; i < children.size(); i++) {
		result += " " + ExpressionTypeToOperator(type) + " " + children[i]->ToString();
	}
	return result;
}

bool ConjunctionExpression::Equals(const ConjunctionExpression *a, const ConjunctionExpression *b) {
	return ExpressionUtil::SetEquals(a->children, b->children);
}

unique_ptr<ParsedExpression> ConjunctionExpression::Copy() const {
	auto copy = make_unique<ConjunctionExpression>(type);
	for (auto &expr : children) {
		copy->children.push_back(expr->Copy());
	}
	copy->CopyProperties(*this);
	return move(copy);
}

void ConjunctionExpression::Serialize(Serializer &serializer) {
	ParsedExpression::Serialize(serializer);
	serializer.WriteList<ParsedExpression>(children);
}

unique_ptr<ParsedExpression> ConjunctionExpression::Deserialize(ExpressionType type, Deserializer &source) {
	auto result = make_unique<ConjunctionExpression>(type);
	source.ReadList<ParsedExpression>(result->children);
	return move(result);
}

} // namespace guinsoodb

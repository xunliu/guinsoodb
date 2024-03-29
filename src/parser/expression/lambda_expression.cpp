#include "guinsoodb/parser/expression/lambda_expression.hpp"
#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/common/serializer.hpp"
#include "guinsoodb/common/types/hash.hpp"

namespace guinsoodb {

LambdaExpression::LambdaExpression(vector<string> parameters, unique_ptr<ParsedExpression> expression)
    : ParsedExpression(ExpressionType::LAMBDA, ExpressionClass::LAMBDA), parameters(move(parameters)),
      expression(move(expression)) {
}

string LambdaExpression::ToString() const {
	string parameter_str;
	if (parameters.size() == 1) {
		parameter_str = parameters[0];
	} else {
		for (auto &parameter : parameters) {
			if (!parameter_str.empty()) {
				parameter_str += ", ";
			}
			parameter_str += parameter;
		}
		parameter_str = "(" + parameter_str + ")";
	}
	return parameter_str + " -> " + expression->ToString();
}

bool LambdaExpression::Equals(const LambdaExpression *a, const LambdaExpression *b) {
	if (a->parameters != b->parameters) {
		return false;
	}
	if (!a->expression->Equals(b->expression.get())) {
		return false;
	}
	return true;
}

hash_t LambdaExpression::Hash() const {
	hash_t result = ParsedExpression::Hash();
	for (auto &parameter : parameters) {
		result = CombineHash(result, guinsoodb::Hash<const char *>(parameter.c_str()));
	}
	result = CombineHash(result, expression->Hash());
	return result;
}

unique_ptr<ParsedExpression> LambdaExpression::Copy() const {
	return make_unique<LambdaExpression>(parameters, expression->Copy());
}

void LambdaExpression::Serialize(Serializer &serializer) {
	ParsedExpression::Serialize(serializer);
	serializer.Write<uint32_t>(parameters.size());
	for (auto &parameter : parameters) {
		serializer.WriteString(parameter);
	}
	expression->Serialize(serializer);
}

unique_ptr<ParsedExpression> LambdaExpression::Deserialize(ExpressionType type, Deserializer &source) {
	auto parameter_count = source.Read<uint32_t>();
	vector<string> parameters;
	parameters.reserve(parameter_count);
	for (size_t i = 0; i < parameter_count; i++) {
		parameters.push_back(source.Read<string>());
	}
	auto expression = ParsedExpression::Deserialize(source);

	return make_unique<LambdaExpression>(move(parameters), move(expression));
}

} // namespace guinsoodb

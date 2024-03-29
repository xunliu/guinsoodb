#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/parser/expression/columnref_expression.hpp"
#include "guinsoodb/parser/expression/lambda_expression.hpp"
#include "guinsoodb/parser/transformer.hpp"

namespace guinsoodb {

static string ExtractColumnFromLambda(ParsedExpression &expr) {
	if (expr.type != ExpressionType::COLUMN_REF) {
		throw ParserException("Lambda parameter must be a column name");
	}
	auto &colref = (ColumnRefExpression &)expr;
	if (!colref.table_name.empty()) {
		throw ParserException("Lambda parameter must be an unqualified name (e.g. 'x', not 'a.x')");
	}
	return colref.column_name;
}

unique_ptr<ParsedExpression> Transformer::TransformLambda(guinsoodb_libpgquery::PGLambdaFunction *node) {
	vector<unique_ptr<ParsedExpression>> parameter_expressions;
	if (!TransformExpressionList(node->parameters, parameter_expressions)) {
		throw ParserException("Failed to transform expression list");
	}
	vector<string> parameters;
	parameters.reserve(parameter_expressions.size());
	for (auto &expr : parameter_expressions) {
		parameters.push_back(ExtractColumnFromLambda(*expr));
	}

	auto lambda_function = TransformExpression(node->function);
	return make_unique<LambdaExpression>(move(parameters), move(lambda_function));
}

} // namespace guinsoodb

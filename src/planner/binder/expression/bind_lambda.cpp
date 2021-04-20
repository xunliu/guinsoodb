#include "guinsoodb/parser/expression/lambda_expression.hpp"
#include "guinsoodb/planner/expression_binder.hpp"

namespace guinsoodb {

BindResult ExpressionBinder::BindExpression(LambdaExpression &expr, idx_t depth) {
	string error;

	// // set up the lambda capture
	// FIXME: need to get the type from the list for binding
	// lambda_capture = expr.capture_name;
	// // bind the child
	// BindChild(expr.expression, depth, error);
	// // clear the lambda capture
	// lambda_capture = "";
	// if (!error.empty()) {
	// 	return BindResult(error);
	// }
	// expr.Print();
	return BindResult("FIXME: bind lambda function");
}

} // namespace guinsoodb

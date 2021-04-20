#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/parser/expression/operator_expression.hpp"
#include "guinsoodb/parser/transformer.hpp"

namespace guinsoodb {

unique_ptr<ParsedExpression> Transformer::TransformNullTest(guinsoodb_libpgquery::PGNullTest *root) {
	D_ASSERT(root);
	auto arg = TransformExpression(reinterpret_cast<guinsoodb_libpgquery::PGNode *>(root->arg));
	if (root->argisrow) {
		throw NotImplementedException("IS NULL argisrow");
	}
	ExpressionType expr_type = (root->nulltesttype == guinsoodb_libpgquery::PG_IS_NULL)
	                               ? ExpressionType::OPERATOR_IS_NULL
	                               : ExpressionType::OPERATOR_IS_NOT_NULL;

	return unique_ptr<ParsedExpression>(new OperatorExpression(expr_type, move(arg)));
}

} // namespace guinsoodb

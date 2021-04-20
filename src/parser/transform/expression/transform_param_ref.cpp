#include "guinsoodb/parser/expression/parameter_expression.hpp"
#include "guinsoodb/parser/transformer.hpp"
#include "guinsoodb/common/algorithm.hpp"

namespace guinsoodb {

unique_ptr<ParsedExpression> Transformer::TransformParamRef(guinsoodb_libpgquery::PGParamRef *node) {
	if (!node) {
		return nullptr;
	}
	auto expr = make_unique<ParameterExpression>();
	if (node->number == 0) {
		expr->parameter_nr = ParamCount() + 1;
	} else {
		expr->parameter_nr = node->number;
	}
	SetParamCount(MaxValue<idx_t>(ParamCount(), expr->parameter_nr));
	return move(expr);
}

} // namespace guinsoodb

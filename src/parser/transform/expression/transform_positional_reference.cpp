#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/parser/transformer.hpp"
#include "guinsoodb/parser/expression/positional_reference_expression.hpp"

namespace guinsoodb {

unique_ptr<ParsedExpression> Transformer::TransformPositionalReference(guinsoodb_libpgquery::PGPositionalReference *node) {
	if (node->position <= 0) {
		throw ParserException("Positional reference node needs to be >= 1");
	}
	auto result = make_unique<PositionalReferenceExpression>(node->position);
	result->query_location = node->location;
	return move(result);
}

} // namespace guinsoodb

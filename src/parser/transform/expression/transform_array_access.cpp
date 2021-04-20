#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/parser/expression/constant_expression.hpp"
#include "guinsoodb/parser/expression/function_expression.hpp"
#include "guinsoodb/parser/expression/operator_expression.hpp"
#include "guinsoodb/parser/transformer.hpp"

namespace guinsoodb {

unique_ptr<ParsedExpression> Transformer::TransformArrayAccess(guinsoodb_libpgquery::PGAIndirection *indirection_node) {
	// transform the source expression
	unique_ptr<ParsedExpression> result;
	result = TransformExpression(indirection_node->arg);

	// now go over the indices
	// note that a single indirection node can contain multiple indices
	// this happens for e.g. more complex accesses (e.g. (foo).field1[42])
	for (auto node = indirection_node->indirection->head; node != nullptr; node = node->next) {
		auto target = reinterpret_cast<guinsoodb_libpgquery::PGNode *>(node->data.ptr_value);
		if (!target) {
			break;
		}
		switch (target->type) {
		case guinsoodb_libpgquery::T_PGAIndices: {
			// index access (either slice or extract)
			auto index = (guinsoodb_libpgquery::PGAIndices *)target;
			vector<unique_ptr<ParsedExpression>> children;
			children.push_back(move(result));
			if (index->is_slice) {
				// slice
				children.push_back(!index->lidx ? make_unique<ConstantExpression>(Value())
				                                : TransformExpression(index->lidx));
				children.push_back(!index->uidx ? make_unique<ConstantExpression>(Value())
				                                : TransformExpression(index->uidx));
				result = make_unique<OperatorExpression>(ExpressionType::ARRAY_SLICE, move(children));
			} else {
				// array access
				D_ASSERT(!index->lidx);
				D_ASSERT(index->uidx);
				children.push_back(TransformExpression(index->uidx));
				result = make_unique<OperatorExpression>(ExpressionType::ARRAY_EXTRACT, move(children));
			}
			break;
		}
		case guinsoodb_libpgquery::T_PGString: {
			auto val = (guinsoodb_libpgquery::PGValue *)target;
			vector<unique_ptr<ParsedExpression>> children;
			children.push_back(move(result));
			children.push_back(TransformValue(*val));
			result = make_unique<OperatorExpression>(ExpressionType::STRUCT_EXTRACT, move(children));
			break;
		}
		case guinsoodb_libpgquery::T_PGAStar:
		default:
			throw NotImplementedException("Unimplemented subscript type");
		}
	}
	return result;
}

} // namespace guinsoodb

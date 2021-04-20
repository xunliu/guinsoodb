#include "guinsoodb/parser/parsed_expression.hpp"
#include "guinsoodb/parser/transformer.hpp"

namespace guinsoodb {

// FIXME: what is the difference between GroupBy and expression list?
bool Transformer::TransformGroupBy(guinsoodb_libpgquery::PGList *group, vector<unique_ptr<ParsedExpression>> &result) {
	if (!group) {
		return false;
	}

	for (auto node = group->head; node != nullptr; node = node->next) {
		auto n = reinterpret_cast<guinsoodb_libpgquery::PGNode *>(node->data.ptr_value);
		result.push_back(TransformExpression(n));
	}
	return true;
}

} // namespace guinsoodb

#include "guinsoodb/parser/tableref/subqueryref.hpp"
#include "guinsoodb/parser/transformer.hpp"

namespace guinsoodb {

unique_ptr<TableRef> Transformer::TransformRangeSubselect(guinsoodb_libpgquery::PGRangeSubselect *root) {
	Transformer subquery_transformer(this);
	auto subquery = subquery_transformer.TransformSelect(root->subquery);
	if (!subquery) {
		return nullptr;
	}
	auto result = make_unique<SubqueryRef>(move(subquery));
	result->alias = TransformAlias(root->alias, result->column_name_alias);
	if (root->sample) {
		result->sample = TransformSampleOptions(root->sample);
	}
	return move(result);
}

} // namespace guinsoodb

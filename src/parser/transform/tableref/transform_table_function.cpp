#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/parser/tableref/table_function_ref.hpp"
#include "guinsoodb/parser/transformer.hpp"

namespace guinsoodb {

unique_ptr<TableRef> Transformer::TransformRangeFunction(guinsoodb_libpgquery::PGRangeFunction *root) {
	if (root->lateral) {
		throw NotImplementedException("LATERAL not implemented");
	}
	if (root->ordinality) {
		throw NotImplementedException("WITH ORDINALITY not implemented");
	}
	if (root->is_rowsfrom) {
		throw NotImplementedException("ROWS FROM() not implemented");
	}
	if (root->functions->length != 1) {
		throw NotImplementedException("Need exactly one function");
	}
	auto function_sublist = (guinsoodb_libpgquery::PGList *)root->functions->head->data.ptr_value;
	D_ASSERT(function_sublist->length == 2);
	auto call_tree = (guinsoodb_libpgquery::PGNode *)function_sublist->head->data.ptr_value;
	auto coldef = function_sublist->head->next->data.ptr_value;

	D_ASSERT(call_tree->type == guinsoodb_libpgquery::T_PGFuncCall);
	if (coldef) {
		throw NotImplementedException("Explicit column definition not supported yet");
	}
	auto func_call = (guinsoodb_libpgquery::PGFuncCall *)call_tree;
	// transform the function call
	auto result = make_unique<TableFunctionRef>();
	result->function = TransformFuncCall(func_call);
	result->alias = TransformAlias(root->alias, result->column_name_alias);
	result->query_location = func_call->location;
	if (root->sample) {
		result->sample = TransformSampleOptions(root->sample);
	}
	return move(result);
}

} // namespace guinsoodb

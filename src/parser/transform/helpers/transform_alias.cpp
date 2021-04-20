#include "guinsoodb/parser/transformer.hpp"

namespace guinsoodb {

string Transformer::TransformAlias(guinsoodb_libpgquery::PGAlias *root, vector<string> &column_name_alias) {
	if (!root) {
		return "";
	}
	if (root->colnames) {
		for (auto node = root->colnames->head; node != nullptr; node = node->next) {
			column_name_alias.emplace_back(
			    reinterpret_cast<guinsoodb_libpgquery::PGValue *>(node->data.ptr_value)->val.str);
		}
	}
	return root->aliasname;
}

} // namespace guinsoodb

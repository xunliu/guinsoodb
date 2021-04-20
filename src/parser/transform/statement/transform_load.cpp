#include "guinsoodb/parser/statement/load_statement.hpp"
#include "guinsoodb/parser/transformer.hpp"

namespace guinsoodb {

unique_ptr<LoadStatement> Transformer::TransformLoad(guinsoodb_libpgquery::PGNode *node) {
	D_ASSERT(node->type == guinsoodb_libpgquery::T_PGLoadStmt);
	auto stmt = reinterpret_cast<guinsoodb_libpgquery::PGLoadStmt *>(node);

	auto load_stmt = make_unique<LoadStatement>();
	auto load_info = make_unique<LoadInfo>();
	load_info->filename = std::string(stmt->filename);
	load_stmt->info = move(load_info);
	return load_stmt;
}

} // namespace guinsoodb

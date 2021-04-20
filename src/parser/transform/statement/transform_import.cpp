#include "guinsoodb/parser/statement/pragma_statement.hpp"
#include "guinsoodb/parser/transformer.hpp"

namespace guinsoodb {

unique_ptr<PragmaStatement> Transformer::TransformImport(guinsoodb_libpgquery::PGNode *node) {
	auto stmt = reinterpret_cast<guinsoodb_libpgquery::PGImportStmt *>(node);
	auto result = make_unique<PragmaStatement>();
	result->info->name = "import_database";
	result->info->parameters.emplace_back(stmt->filename);
	return result;
}

} // namespace guinsoodb

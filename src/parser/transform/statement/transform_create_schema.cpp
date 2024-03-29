#include "guinsoodb/parser/statement/create_statement.hpp"
#include "guinsoodb/parser/parsed_data/create_schema_info.hpp"
#include "guinsoodb/parser/transformer.hpp"

namespace guinsoodb {

unique_ptr<CreateStatement> Transformer::TransformCreateSchema(guinsoodb_libpgquery::PGNode *node) {
	auto stmt = reinterpret_cast<guinsoodb_libpgquery::PGCreateSchemaStmt *>(node);
	D_ASSERT(stmt);
	auto result = make_unique<CreateStatement>();
	auto info = make_unique<CreateSchemaInfo>();

	D_ASSERT(stmt->schemaname);
	info->schema = stmt->schemaname;
	info->on_conflict =
	    stmt->if_not_exists ? OnCreateConflict::IGNORE_ON_CONFLICT : OnCreateConflict::ERROR_ON_CONFLICT;

	if (stmt->schemaElts) {
		// schema elements
		for (auto cell = stmt->schemaElts->head; cell != nullptr; cell = cell->next) {
			auto node = reinterpret_cast<guinsoodb_libpgquery::PGNode *>(cell->data.ptr_value);
			switch (node->type) {
			case guinsoodb_libpgquery::T_PGCreateStmt:
			case guinsoodb_libpgquery::T_PGViewStmt:
			default:
				throw NotImplementedException("Schema element not supported yet!");
			}
		}
	}
	result->info = move(info);
	return result;
}

} // namespace guinsoodb

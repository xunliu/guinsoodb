#include "guinsoodb/parser/statement/create_statement.hpp"
#include "guinsoodb/parser/parsed_data/create_table_info.hpp"
#include "guinsoodb/parser/transformer.hpp"

namespace guinsoodb {

unique_ptr<CreateStatement> Transformer::TransformCreateTableAs(guinsoodb_libpgquery::PGNode *node) {
	auto stmt = reinterpret_cast<guinsoodb_libpgquery::PGCreateTableAsStmt *>(node);
	D_ASSERT(stmt);
	if (stmt->relkind == guinsoodb_libpgquery::PG_OBJECT_MATVIEW) {
		throw NotImplementedException("Materialized view not implemented");
	}
	if (stmt->is_select_into || stmt->into->colNames || stmt->into->options) {
		throw NotImplementedException("Unimplemented features for CREATE TABLE as");
	}
	auto qname = TransformQualifiedName(stmt->into->rel);
	auto query = TransformSelect(stmt->query, false);

	auto result = make_unique<CreateStatement>();
	auto info = make_unique<CreateTableInfo>();
	info->schema = qname.schema;
	info->table = qname.name;
	info->on_conflict =
	    stmt->if_not_exists ? OnCreateConflict::IGNORE_ON_CONFLICT : OnCreateConflict::ERROR_ON_CONFLICT;
	info->temporary =
	    stmt->into->rel->relpersistence == guinsoodb_libpgquery::PGPostgresRelPersistence::PG_RELPERSISTENCE_TEMP;
	info->query = move(query);
	result->info = move(info);
	return result;
}

} // namespace guinsoodb

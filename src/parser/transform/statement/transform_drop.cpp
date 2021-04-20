#include "guinsoodb/parser/statement/drop_statement.hpp"
#include "guinsoodb/parser/transformer.hpp"

namespace guinsoodb {

unique_ptr<SQLStatement> Transformer::TransformDrop(guinsoodb_libpgquery::PGNode *node) {
	auto stmt = (guinsoodb_libpgquery::PGDropStmt *)(node);
	auto result = make_unique<DropStatement>();
	auto &info = *result->info.get();
	D_ASSERT(stmt);
	if (stmt->objects->length != 1) {
		throw NotImplementedException("Can only drop one object at a time");
	}
	switch (stmt->removeType) {
	case guinsoodb_libpgquery::PG_OBJECT_TABLE:
		info.type = CatalogType::TABLE_ENTRY;
		break;
	case guinsoodb_libpgquery::PG_OBJECT_SCHEMA:
		info.type = CatalogType::SCHEMA_ENTRY;
		break;
	case guinsoodb_libpgquery::PG_OBJECT_INDEX:
		info.type = CatalogType::INDEX_ENTRY;
		break;
	case guinsoodb_libpgquery::PG_OBJECT_VIEW:
		info.type = CatalogType::VIEW_ENTRY;
		break;
	case guinsoodb_libpgquery::PG_OBJECT_SEQUENCE:
		info.type = CatalogType::SEQUENCE_ENTRY;
		break;
	case guinsoodb_libpgquery::PG_OBJECT_FUNCTION:
		info.type = CatalogType::MACRO_ENTRY;
		break;
	default:
		throw NotImplementedException("Cannot drop this type yet");
	}

	switch (stmt->removeType) {
	case guinsoodb_libpgquery::PG_OBJECT_SCHEMA:
		D_ASSERT(stmt->objects && stmt->objects->length == 1);
		info.name = ((guinsoodb_libpgquery::PGValue *)stmt->objects->head->data.ptr_value)->val.str;
		break;
	default: {
		auto view_list = (guinsoodb_libpgquery::PGList *)stmt->objects->head->data.ptr_value;
		if (view_list->length == 2) {
			info.schema = ((guinsoodb_libpgquery::PGValue *)view_list->head->data.ptr_value)->val.str;
			info.name = ((guinsoodb_libpgquery::PGValue *)view_list->head->next->data.ptr_value)->val.str;
		} else {
			info.name = ((guinsoodb_libpgquery::PGValue *)view_list->head->data.ptr_value)->val.str;
		}
		break;
	}
	}
	info.cascade = stmt->behavior == guinsoodb_libpgquery::PGDropBehavior::PG_DROP_CASCADE;
	info.if_exists = stmt->missing_ok;
	return move(result);
}

} // namespace guinsoodb

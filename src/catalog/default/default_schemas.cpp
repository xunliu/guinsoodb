#include "guinsoodb/catalog/default/default_schemas.hpp"
#include "guinsoodb/catalog/catalog_entry/schema_catalog_entry.hpp"

namespace guinsoodb {

struct DefaultSchema {
	const char *name;
};

static DefaultSchema internal_schemas[] = {{"information_schema"}, {nullptr}};

static bool GetDefaultSchema(const string &schema) {
	for (idx_t index = 0; internal_schemas[index].name != nullptr; index++) {
		if (internal_schemas[index].name == schema) {
			return true;
		}
	}
	return false;
}

DefaultSchemaGenerator::DefaultSchemaGenerator(Catalog &catalog) : DefaultGenerator(catalog) {
}

unique_ptr<CatalogEntry> DefaultSchemaGenerator::CreateDefaultEntry(ClientContext &context, const string &entry_name) {
	if (GetDefaultSchema(entry_name)) {
		return make_unique_base<CatalogEntry, SchemaCatalogEntry>(&catalog, entry_name, true);
	}
	return nullptr;
}

} // namespace guinsoodb

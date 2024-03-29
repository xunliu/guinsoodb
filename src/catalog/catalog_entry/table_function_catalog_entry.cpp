#include "guinsoodb/catalog/catalog_entry/table_function_catalog_entry.hpp"
#include "guinsoodb/parser/parsed_data/create_table_function_info.hpp"

namespace guinsoodb {

TableFunctionCatalogEntry::TableFunctionCatalogEntry(Catalog *catalog, SchemaCatalogEntry *schema,
                                                     CreateTableFunctionInfo *info)
    : StandardEntry(CatalogType::TABLE_FUNCTION_ENTRY, schema, catalog, info->name), functions(move(info->functions)) {
	D_ASSERT(this->functions.size() > 0);
}

} // namespace guinsoodb

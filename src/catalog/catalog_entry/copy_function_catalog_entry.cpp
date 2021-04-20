#include "guinsoodb/catalog/catalog_entry/copy_function_catalog_entry.hpp"
#include "guinsoodb/parser/parsed_data/create_copy_function_info.hpp"

namespace guinsoodb {

CopyFunctionCatalogEntry::CopyFunctionCatalogEntry(Catalog *catalog, SchemaCatalogEntry *schema,
                                                   CreateCopyFunctionInfo *info)
    : StandardEntry(CatalogType::COPY_FUNCTION_ENTRY, schema, catalog, info->name), function(info->function) {
}

} // namespace guinsoodb

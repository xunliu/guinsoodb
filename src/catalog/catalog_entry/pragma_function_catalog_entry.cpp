#include "guinsoodb/catalog/catalog_entry/pragma_function_catalog_entry.hpp"
#include "guinsoodb/parser/parsed_data/create_pragma_function_info.hpp"

namespace guinsoodb {

PragmaFunctionCatalogEntry::PragmaFunctionCatalogEntry(Catalog *catalog, SchemaCatalogEntry *schema,
                                                       CreatePragmaFunctionInfo *info)
    : StandardEntry(CatalogType::PRAGMA_FUNCTION_ENTRY, schema, catalog, info->name), functions(move(info->functions)) {
}

} // namespace guinsoodb

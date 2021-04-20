//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/catalog/catalog_entry/scalar_function_catalog_entry.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/catalog/standard_entry.hpp"
#include "guinsoodb/catalog/catalog_set.hpp"
#include "guinsoodb/function/function.hpp"
#include "guinsoodb/parser/parsed_data/create_scalar_function_info.hpp"

namespace guinsoodb {

//! A table function in the catalog
class ScalarFunctionCatalogEntry : public StandardEntry {
public:
	ScalarFunctionCatalogEntry(Catalog *catalog, SchemaCatalogEntry *schema, CreateScalarFunctionInfo *info)
	    : StandardEntry(CatalogType::SCALAR_FUNCTION_ENTRY, schema, catalog, info->name), functions(info->functions) {
	}

	//! The scalar functions
	vector<ScalarFunction> functions;
};
} // namespace guinsoodb

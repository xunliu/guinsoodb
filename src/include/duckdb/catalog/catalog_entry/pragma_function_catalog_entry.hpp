//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/catalog/catalog_entry/pragma_function_catalog_entry.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/catalog/standard_entry.hpp"
#include "guinsoodb/function/pragma_function.hpp"

namespace guinsoodb {

class Catalog;
struct CreatePragmaFunctionInfo;

//! A table function in the catalog
class PragmaFunctionCatalogEntry : public StandardEntry {
public:
	PragmaFunctionCatalogEntry(Catalog *catalog, SchemaCatalogEntry *schema, CreatePragmaFunctionInfo *info);

	//! The pragma functions
	vector<PragmaFunction> functions;
};
} // namespace guinsoodb

//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/catalog/catalog_entry/copy_function_catalog_entry.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/catalog/standard_entry.hpp"
#include "guinsoodb/common/unordered_map.hpp"
#include "guinsoodb/function/copy_function.hpp"

namespace guinsoodb {

class Catalog;
struct CreateCopyFunctionInfo;

//! A table function in the catalog
class CopyFunctionCatalogEntry : public StandardEntry {
public:
	CopyFunctionCatalogEntry(Catalog *catalog, SchemaCatalogEntry *schema, CreateCopyFunctionInfo *info);

	//! The copy function
	CopyFunction function;
};
} // namespace guinsoodb

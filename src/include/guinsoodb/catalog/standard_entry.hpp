//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/catalog/standard_entry.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/catalog/catalog_entry.hpp"

namespace guinsoodb {
class SchemaCatalogEntry;

//! A StandardEntry is a catalog entry that is a member of a schema
class StandardEntry : public CatalogEntry {
public:
	StandardEntry(CatalogType type, SchemaCatalogEntry *schema, Catalog *catalog, string name)
	    : CatalogEntry(type, catalog, name), schema(schema) {
	}
	~StandardEntry() override {
	}

	//! The schema the entry belongs to
	SchemaCatalogEntry *schema;
};
} // namespace guinsoodb

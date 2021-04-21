//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/catalog/catalog_entry/macro_catalog_entry.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/catalog/catalog_set.hpp"
#include "guinsoodb/catalog/standard_entry.hpp"
#include "guinsoodb/function/macro_function.hpp"
#include "guinsoodb/parser/parsed_data/create_macro_info.hpp"

namespace guinsoodb {

//! A macro function in the catalog
class MacroCatalogEntry : public StandardEntry {
public:
	MacroCatalogEntry(Catalog *catalog, SchemaCatalogEntry *schema, CreateMacroInfo *info)
	    : StandardEntry(CatalogType::MACRO_ENTRY, schema, catalog, info->name), function(move(info->function)) {
	}

	//! The macro function
	unique_ptr<MacroFunction> function;

public:
	//! Serialize the meta information of the MacroCatalogEntry a serializer
	virtual void Serialize(Serializer &serializer);
	//! Deserializes to a CreateMacroInfo
	static unique_ptr<CreateMacroInfo> Deserialize(Deserializer &source);
};
} // namespace guinsoodb

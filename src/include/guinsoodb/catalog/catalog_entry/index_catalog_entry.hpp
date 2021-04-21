//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/catalog/catalog_entry/index_catalog_entry.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/catalog/standard_entry.hpp"
#include "guinsoodb/parser/parsed_data/create_index_info.hpp"

namespace guinsoodb {

struct DataTableInfo;
class Index;

//! An index catalog entry
class IndexCatalogEntry : public StandardEntry {
public:
	//! Create a real TableCatalogEntry and initialize storage for it
	IndexCatalogEntry(Catalog *catalog, SchemaCatalogEntry *schema, CreateIndexInfo *info);
	~IndexCatalogEntry() override;

	Index *index;
	shared_ptr<DataTableInfo> info;
	string sql;

public:
	string ToSQL() override;
};

} // namespace guinsoodb

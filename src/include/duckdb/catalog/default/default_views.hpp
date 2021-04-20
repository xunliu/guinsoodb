//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/catalog/default/default_views.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_data/create_view_info.hpp"
#include "guinsoodb/catalog/default/default_generator.hpp"

namespace guinsoodb {
class SchemaCatalogEntry;

class DefaultViewGenerator : public DefaultGenerator {
public:
	DefaultViewGenerator(Catalog &catalog, SchemaCatalogEntry *schema);

	SchemaCatalogEntry *schema;

public:
	unique_ptr<CatalogEntry> CreateDefaultEntry(ClientContext &context, const string &entry_name) override;
};

} // namespace guinsoodb

//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/catalog/default/default_functions.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/catalog/default/default_generator.hpp"

namespace guinsoodb {
class SchemaCatalogEntry;

class DefaultFunctionGenerator : public DefaultGenerator {
public:
	DefaultFunctionGenerator(Catalog &catalog, SchemaCatalogEntry *schema);

	SchemaCatalogEntry *schema;

public:
	unique_ptr<CatalogEntry> CreateDefaultEntry(ClientContext &context, const string &entry_name) override;
};

} // namespace guinsoodb

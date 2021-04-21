//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/catalog/default/default_schemas.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/catalog/default/default_generator.hpp"

namespace guinsoodb {

class DefaultSchemaGenerator : public DefaultGenerator {
public:
	explicit DefaultSchemaGenerator(Catalog &catalog);

public:
	unique_ptr<CatalogEntry> CreateDefaultEntry(ClientContext &context, const string &entry_name) override;
};

} // namespace guinsoodb

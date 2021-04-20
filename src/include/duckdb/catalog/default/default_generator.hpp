//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/catalog/default/default_generator.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/catalog/catalog_entry.hpp"

namespace guinsoodb {
class ClientContext;

class DefaultGenerator {
public:
	explicit DefaultGenerator(Catalog &catalog) : catalog(catalog) {
	}
	virtual ~DefaultGenerator() {
	}

	Catalog &catalog;

public:
	//! Creates a default entry with the specified name, or returns nullptr if no such entry can be generated
	virtual unique_ptr<CatalogEntry> CreateDefaultEntry(ClientContext &context, const string &entry_name) = 0;
};

} // namespace guinsoodb

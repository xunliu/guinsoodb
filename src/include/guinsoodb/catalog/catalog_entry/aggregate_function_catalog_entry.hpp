//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/catalog/catalog_entry/aggregate_function_catalog_entry.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/catalog/standard_entry.hpp"
#include "guinsoodb/catalog/catalog_set.hpp"
#include "guinsoodb/function/function.hpp"
#include "guinsoodb/parser/parsed_data/create_aggregate_function_info.hpp"

namespace guinsoodb {

//! An aggregate function in the catalog
class AggregateFunctionCatalogEntry : public StandardEntry {
public:
	AggregateFunctionCatalogEntry(Catalog *catalog, SchemaCatalogEntry *schema, CreateAggregateFunctionInfo *info)
	    : StandardEntry(CatalogType::AGGREGATE_FUNCTION_ENTRY, schema, catalog, info->name),
	      functions(info->functions.functions) {
	}

	//! The aggregate functions
	vector<AggregateFunction> functions;
};
} // namespace guinsoodb

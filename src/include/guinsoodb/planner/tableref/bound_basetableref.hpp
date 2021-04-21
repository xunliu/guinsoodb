//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/tableref/bound_basetableref.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/planner/bound_tableref.hpp"
#include "guinsoodb/planner/logical_operator.hpp"

namespace guinsoodb {
class TableCatalogEntry;

//! Represents a TableReference to a base table in the schema
class BoundBaseTableRef : public BoundTableRef {
public:
	BoundBaseTableRef(TableCatalogEntry *table, unique_ptr<LogicalOperator> get)
	    : BoundTableRef(TableReferenceType::BASE_TABLE), table(table), get(move(get)) {
	}

	TableCatalogEntry *table;
	unique_ptr<LogicalOperator> get;
};
} // namespace guinsoodb

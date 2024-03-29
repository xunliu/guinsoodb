//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/parsed_data/bound_create_table_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_data/create_table_info.hpp"
#include "guinsoodb/planner/bound_constraint.hpp"
#include "guinsoodb/planner/expression.hpp"
#include "guinsoodb/planner/logical_operator.hpp"
#include "guinsoodb/storage/table/persistent_table_data.hpp"

namespace guinsoodb {
class CatalogEntry;

struct BoundCreateTableInfo {
	explicit BoundCreateTableInfo(unique_ptr<CreateInfo> base) : base(move(base)) {
	}

	//! The schema to create the table in
	SchemaCatalogEntry *schema;
	//! The base CreateInfo object
	unique_ptr<CreateInfo> base;
	//! The map of column names -> column index, used during binding
	unordered_map<string, column_t> name_map;
	//! List of constraints on the table
	vector<unique_ptr<Constraint>> constraints;
	//! List of bound constraints on the table
	vector<unique_ptr<BoundConstraint>> bound_constraints;
	//! Bound default values
	vector<unique_ptr<Expression>> bound_defaults;
	//! Dependents of the table (in e.g. default values)
	unordered_set<CatalogEntry *> dependencies;
	//! The existing table data on disk (if any)
	unique_ptr<PersistentTableData> data;
	//! CREATE TABLE from QUERY
	unique_ptr<LogicalOperator> query;

	CreateTableInfo &Base() {
		return (CreateTableInfo &)*base;
	}
};

} // namespace guinsoodb

//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/main/table_description.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/column_definition.hpp"

namespace guinsoodb {

struct TableDescription {
	//! The schema of the table
	string schema;
	//! The table name of the table
	string table;
	//! The columns of the table
	vector<ColumnDefinition> columns;
};

} // namespace guinsoodb

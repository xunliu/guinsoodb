//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/tableref/bound_table_function.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/planner/bound_tableref.hpp"
#include "guinsoodb/planner/logical_operator.hpp"

namespace guinsoodb {

//! Represents a reference to a table-producing function call
class BoundTableFunction : public BoundTableRef {
public:
	explicit BoundTableFunction(unique_ptr<LogicalOperator> get)
	    : BoundTableRef(TableReferenceType::TABLE_FUNCTION), get(move(get)) {
	}

	unique_ptr<LogicalOperator> get;
};

} // namespace guinsoodb

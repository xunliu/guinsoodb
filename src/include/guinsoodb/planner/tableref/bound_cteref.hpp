//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/tableref/bound_cteref.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/planner/bound_tableref.hpp"

namespace guinsoodb {

class BoundCTERef : public BoundTableRef {
public:
	BoundCTERef(idx_t bind_index, idx_t cte_index)
	    : BoundTableRef(TableReferenceType::CTE), bind_index(bind_index), cte_index(cte_index) {
	}

	//! The set of columns bound to this base table reference
	vector<string> bound_columns;
	//! The types of the values list
	vector<LogicalType> types;
	//! The index in the bind context
	idx_t bind_index;
	//! The index of the cte
	idx_t cte_index;
};
} // namespace guinsoodb

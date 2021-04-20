//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/column_binding.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/common.hpp"

namespace guinsoodb {

struct ColumnBinding {
	idx_t table_index;
	idx_t column_index;

	ColumnBinding() : table_index(INVALID_INDEX), column_index(INVALID_INDEX) {
	}
	ColumnBinding(idx_t table, idx_t column) : table_index(table), column_index(column) {
	}

	bool operator==(const ColumnBinding &rhs) const {
		return table_index == rhs.table_index && column_index == rhs.column_index;
	}
};

} // namespace guinsoodb

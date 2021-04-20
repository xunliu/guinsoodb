//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/tableref/bound_dummytableref.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/planner/bound_tableref.hpp"

namespace guinsoodb {

//! Represents a cross product
class BoundEmptyTableRef : public BoundTableRef {
public:
	explicit BoundEmptyTableRef(idx_t bind_index) : BoundTableRef(TableReferenceType::EMPTY), bind_index(bind_index) {
	}
	idx_t bind_index;
};
} // namespace guinsoodb

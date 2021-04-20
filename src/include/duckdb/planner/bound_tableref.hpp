//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/bound_tableref.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/common.hpp"
#include "guinsoodb/common/enums/tableref_type.hpp"
#include "guinsoodb/parser/parsed_data/sample_options.hpp"

namespace guinsoodb {

class BoundTableRef {
public:
	explicit BoundTableRef(TableReferenceType type) : type(type) {
	}
	virtual ~BoundTableRef() {
	}

	//! The type of table reference
	TableReferenceType type;
	//! The sample options (if any)
	unique_ptr<SampleOptions> sample;
};
} // namespace guinsoodb

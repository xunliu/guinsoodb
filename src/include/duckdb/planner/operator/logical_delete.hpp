//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/operator/logical_delete.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/planner/logical_operator.hpp"

namespace guinsoodb {

class LogicalDelete : public LogicalOperator {
public:
	explicit LogicalDelete(TableCatalogEntry *table)
	    : LogicalOperator(LogicalOperatorType::LOGICAL_DELETE), table(table) {
	}

	TableCatalogEntry *table;

protected:
	void ResolveTypes() override {
		types.push_back(LogicalType::BIGINT);
	}
};
} // namespace guinsoodb

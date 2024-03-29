//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/operator/logical_unnest.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/planner/logical_operator.hpp"

namespace guinsoodb {

//! LogicalAggregate represents an aggregate operation with (optional) GROUP BY
//! operator.
class LogicalUnnest : public LogicalOperator {
public:
	explicit LogicalUnnest(idx_t unnest_index)
	    : LogicalOperator(LogicalOperatorType::LOGICAL_UNNEST), unnest_index(unnest_index) {
	}

	idx_t unnest_index;

public:
	vector<ColumnBinding> GetColumnBindings() override;

protected:
	void ResolveTypes() override;
};
} // namespace guinsoodb

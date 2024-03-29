//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/operator/logical_window.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/planner/logical_operator.hpp"

namespace guinsoodb {

//! LogicalAggregate represents an aggregate operation with (optional) GROUP BY
//! operator.
class LogicalWindow : public LogicalOperator {
public:
	explicit LogicalWindow(idx_t window_index)
	    : LogicalOperator(LogicalOperatorType::LOGICAL_WINDOW), window_index(window_index) {
	}

	idx_t window_index;

public:
	vector<ColumnBinding> GetColumnBindings() override;

protected:
	void ResolveTypes() override;
};
} // namespace guinsoodb

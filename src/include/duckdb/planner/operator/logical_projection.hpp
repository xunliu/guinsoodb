//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/operator/logical_projection.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/planner/logical_operator.hpp"

namespace guinsoodb {

//! LogicalProjection represents the projection list in a SELECT clause
class LogicalProjection : public LogicalOperator {
public:
	LogicalProjection(idx_t table_index, vector<unique_ptr<Expression>> select_list);

	idx_t table_index;

public:
	vector<ColumnBinding> GetColumnBindings() override;

protected:
	void ResolveTypes() override;
};
} // namespace guinsoodb

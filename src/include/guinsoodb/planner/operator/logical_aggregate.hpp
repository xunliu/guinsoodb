//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/operator/logical_aggregate.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/planner/logical_operator.hpp"
#include "guinsoodb/planner/column_binding.hpp"
#include "guinsoodb/storage/statistics/base_statistics.hpp"
namespace guinsoodb {

//! LogicalAggregate represents an aggregate operation with (optional) GROUP BY
//! operator.
class LogicalAggregate : public LogicalOperator {
public:
	LogicalAggregate(idx_t group_index, idx_t aggregate_index, vector<unique_ptr<Expression>> select_list);

	//! The table index for the groups of the LogicalAggregate
	idx_t group_index;
	//! The table index for the aggregates of the LogicalAggregate
	idx_t aggregate_index;
	//! The set of groups (optional).
	vector<unique_ptr<Expression>> groups;
	//! Group statistics (optional)
	vector<unique_ptr<BaseStatistics>> group_stats;

public:
	string ParamsToString() const override;

	vector<ColumnBinding> GetColumnBindings() override;

protected:
	void ResolveTypes() override;
};
} // namespace guinsoodb

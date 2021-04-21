//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/operator/logical_comparison_join.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/constants.hpp"
#include "guinsoodb/common/unordered_set.hpp"
#include "guinsoodb/planner/joinside.hpp"
#include "guinsoodb/planner/operator/logical_join.hpp"

namespace guinsoodb {

//! LogicalComparisonJoin represents a join that involves comparisons between the LHS and RHS
class LogicalComparisonJoin : public LogicalJoin {
public:
	explicit LogicalComparisonJoin(JoinType type,
	                               LogicalOperatorType logical_type = LogicalOperatorType::LOGICAL_COMPARISON_JOIN);

	//! The conditions of the join
	vector<JoinCondition> conditions;
	//! Used for duplicate-eliminated joins
	vector<LogicalType> delim_types;

public:
	string ParamsToString() const override;

public:
	static unique_ptr<LogicalOperator> CreateJoin(JoinType type, unique_ptr<LogicalOperator> left_child,
	                                              unique_ptr<LogicalOperator> right_child,
	                                              unordered_set<idx_t> &left_bindings,
	                                              unordered_set<idx_t> &right_bindings,
	                                              vector<unique_ptr<Expression>> &expressions);
};

} // namespace guinsoodb

//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/operator/logical_filter.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/planner/logical_operator.hpp"

namespace guinsoodb {

//! LogicalFilter represents a filter operation (e.g. WHERE or HAVING clause)
class LogicalFilter : public LogicalOperator {
public:
	explicit LogicalFilter(unique_ptr<Expression> expression);
	LogicalFilter();

	vector<idx_t> projection_map;

public:
	vector<ColumnBinding> GetColumnBindings() override;

	bool SplitPredicates() {
		return SplitPredicates(expressions);
	}
	//! Splits up the predicates of the LogicalFilter into a set of predicates
	//! separated by AND Returns whether or not any splits were made
	static bool SplitPredicates(vector<unique_ptr<Expression>> &expressions);

protected:
	void ResolveTypes() override;
};

} // namespace guinsoodb

//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/optimizer/common_aggregate_optimizer.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/planner/logical_operator_visitor.hpp"
#include "guinsoodb/planner/column_binding_map.hpp"

namespace guinsoodb {
//! The CommonAggregateOptimizer optimizer eliminates duplicate aggregates from aggregate nodes
class CommonAggregateOptimizer : public LogicalOperatorVisitor {
public:
	void VisitOperator(LogicalOperator &op) override;

private:
	unique_ptr<Expression> VisitReplace(BoundColumnRefExpression &expr, unique_ptr<Expression> *expr_ptr) override;

	void ExtractCommonAggregates(LogicalAggregate &aggr);

private:
	column_binding_map_t<ColumnBinding> aggregate_map;
};
} // namespace guinsoodb

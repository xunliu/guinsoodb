//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/execution/column_binding_resolver.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/planner/logical_operator_visitor.hpp"
#include "guinsoodb/planner/column_binding_map.hpp"
#include "guinsoodb/common/vector.hpp"

namespace guinsoodb {

//! The ColumnBindingResolver resolves ColumnBindings into base tables
//! (table_index, column_index) into physical indices into the DataChunks that
//! are used within the execution engine
class ColumnBindingResolver : public LogicalOperatorVisitor {
public:
	ColumnBindingResolver();

	void VisitOperator(LogicalOperator &op) override;

protected:
	vector<ColumnBinding> bindings;

	unique_ptr<Expression> VisitReplace(BoundColumnRefExpression &expr, unique_ptr<Expression> *expr_ptr) override;
};
} // namespace guinsoodb

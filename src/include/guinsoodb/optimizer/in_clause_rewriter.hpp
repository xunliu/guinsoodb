//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/optimizer/in_clause_rewriter.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/planner/logical_operator_visitor.hpp"

namespace guinsoodb {
class Optimizer;

class InClauseRewriter : public LogicalOperatorVisitor {
public:
	explicit InClauseRewriter(Optimizer &optimizer) : optimizer(optimizer) {
	}

	Optimizer &optimizer;
	unique_ptr<LogicalOperator> root;

public:
	unique_ptr<LogicalOperator> Rewrite(unique_ptr<LogicalOperator> op);

	unique_ptr<Expression> VisitReplace(BoundOperatorExpression &expr, unique_ptr<Expression> *expr_ptr) override;
};

} // namespace guinsoodb

//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/optimizer/optimizer.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/optimizer/expression_rewriter.hpp"
#include "guinsoodb/planner/logical_operator.hpp"
#include "guinsoodb/planner/logical_operator_visitor.hpp"

namespace guinsoodb {
class Binder;

class Optimizer {
public:
	Optimizer(Binder &binder, ClientContext &context);

	unique_ptr<LogicalOperator> Optimize(unique_ptr<LogicalOperator> plan);

	ClientContext &context;
	Binder &binder;
	ExpressionRewriter rewriter;
};

} // namespace guinsoodb

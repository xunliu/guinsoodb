//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/optimizer/rule/arithmetic_simplification.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/optimizer/rule.hpp"

namespace guinsoodb {

// The Arithmetic Simplification rule applies arithmetic expressions to which the answer is known (e.g. X + 0 => X, X *
// 0 => 0)
class ArithmeticSimplificationRule : public Rule {
public:
	explicit ArithmeticSimplificationRule(ExpressionRewriter &rewriter);

	unique_ptr<Expression> Apply(LogicalOperator &op, vector<Expression *> &bindings, bool &changes_made) override;
};

} // namespace guinsoodb

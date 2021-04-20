//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/optimizer/rule.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/optimizer/matcher/expression_matcher.hpp"
#include "guinsoodb/optimizer/matcher/logical_operator_matcher.hpp"

namespace guinsoodb {
class ExpressionRewriter;

class Rule {
public:
	explicit Rule(ExpressionRewriter &rewriter) : rewriter(rewriter) {
	}
	virtual ~Rule() {
	}

	//! The expression rewriter this rule belongs to
	ExpressionRewriter &rewriter;
	//! The root
	unique_ptr<LogicalOperatorMatcher> logical_root;
	//! The expression matcher of the rule
	unique_ptr<ExpressionMatcher> root;

	virtual unique_ptr<Expression> Apply(LogicalOperator &op, vector<Expression *> &bindings, bool &fixed_point) = 0;
};

} // namespace guinsoodb

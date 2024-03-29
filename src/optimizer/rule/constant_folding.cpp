#include "guinsoodb/optimizer/rule/constant_folding.hpp"

#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/execution/expression_executor.hpp"
#include "guinsoodb/optimizer/expression_rewriter.hpp"
#include "guinsoodb/planner/expression/bound_constant_expression.hpp"

namespace guinsoodb {

//! The ConstantFoldingExpressionMatcher matches on any scalar expression (i.e. Expression::IsFoldable is true)
class ConstantFoldingExpressionMatcher : public FoldableConstantMatcher {
public:
	bool Match(Expression *expr, vector<Expression *> &bindings) override {
		// we also do not match on ConstantExpressions, because we cannot fold those any further
		if (expr->type == ExpressionType::VALUE_CONSTANT) {
			return false;
		}
		return FoldableConstantMatcher::Match(expr, bindings);
	}
};

ConstantFoldingRule::ConstantFoldingRule(ExpressionRewriter &rewriter) : Rule(rewriter) {
	auto op = make_unique<ConstantFoldingExpressionMatcher>();
	root = move(op);
}

unique_ptr<Expression> ConstantFoldingRule::Apply(LogicalOperator &op, vector<Expression *> &bindings,
                                                  bool &changes_made) {
	auto root = bindings[0];
	// the root is a scalar expression that we have to fold
	D_ASSERT(root->IsFoldable() && root->type != ExpressionType::VALUE_CONSTANT);

	// use an ExpressionExecutor to execute the expression
	auto result_value = ExpressionExecutor::EvaluateScalar(*root);
	D_ASSERT(result_value.type() == root->return_type);
	// now get the value from the result vector and insert it back into the plan as a constant expression
	return make_unique<BoundConstantExpression>(result_value);
}

} // namespace guinsoodb

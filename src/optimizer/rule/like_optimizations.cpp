#include "guinsoodb/optimizer/rule/like_optimizations.hpp"

#include "guinsoodb/execution/expression_executor.hpp"
#include "guinsoodb/planner/expression/bound_function_expression.hpp"
#include "guinsoodb/planner/expression/bound_constant_expression.hpp"

#include "re2/re2.h"

namespace guinsoodb {

LikeOptimizationRule::LikeOptimizationRule(ExpressionRewriter &rewriter) : Rule(rewriter) {
	// match on a FunctionExpression that has a foldable ConstantExpression
	auto func = make_unique<FunctionExpressionMatcher>();
	func->matchers.push_back(make_unique<ConstantExpressionMatcher>());
	func->matchers.push_back(make_unique<ExpressionMatcher>());
	func->policy = SetMatcher::Policy::SOME;
	// we only match on LIKE ("~~")
	func->function = make_unique<SpecificFunctionMatcher>("~~");
	root = move(func);
}

unique_ptr<Expression> LikeOptimizationRule::Apply(LogicalOperator &op, vector<Expression *> &bindings,
                                                   bool &changes_made) {
	auto root = (BoundFunctionExpression *)bindings[0];
	auto constant_expr = (BoundConstantExpression *)bindings[1];
	D_ASSERT(root->children.size() == 2);

	if (constant_expr->value.is_null) {
		return make_unique<BoundConstantExpression>(Value(root->return_type));
	}

	// the constant_expr is a scalar expression that we have to fold
	if (!constant_expr->IsFoldable()) {
		return root->Copy();
	}

	auto constant_value = ExpressionExecutor::EvaluateScalar(*constant_expr);
	D_ASSERT(constant_value.type() == constant_expr->return_type);
	auto patt_str = constant_value.str_value;

	guinsoodb_re2::RE2 prefix_pattern("[^%_]*[%]+");
	guinsoodb_re2::RE2 suffix_pattern("[%]+[^%_]*");
	guinsoodb_re2::RE2 contains_pattern("[%]+[^%_]*[%]+");

	if (guinsoodb_re2::RE2::FullMatch(patt_str, prefix_pattern)) {
		// Prefix LIKE pattern : [^%_]*[%]+, ignoring underscore
		return ApplyRule(root, PrefixFun::GetFunction(), patt_str);
	} else if (guinsoodb_re2::RE2::FullMatch(patt_str, suffix_pattern)) {
		// Suffix LIKE pattern: [%]+[^%_]*, ignoring underscore
		return ApplyRule(root, SuffixFun::GetFunction(), patt_str);
	} else if (guinsoodb_re2::RE2::FullMatch(patt_str, contains_pattern)) {
		// Contains LIKE pattern: [%]+[^%_]*[%]+, ignoring underscore
		return ApplyRule(root, ContainsFun::GetFunction(), patt_str);
	}

	return nullptr;
}

unique_ptr<Expression> LikeOptimizationRule::ApplyRule(BoundFunctionExpression *expr, ScalarFunction function,
                                                       string pattern) {
	// replace LIKE by an optimized function
	expr->function = move(function);

	// removing "%" from the pattern
	pattern.erase(std::remove(pattern.begin(), pattern.end(), '%'), pattern.end());

	expr->children[1] = make_unique<BoundConstantExpression>(Value(move(pattern)));

	return expr->Copy();
}

} // namespace guinsoodb

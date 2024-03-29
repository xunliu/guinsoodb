#include "guinsoodb/optimizer/rule/empty_needle_removal.hpp"

#include "guinsoodb/execution/expression_executor.hpp"
#include "guinsoodb/planner/expression/bound_function_expression.hpp"
#include "guinsoodb/planner/expression/bound_constant_expression.hpp"
#include "guinsoodb/planner/expression/bound_operator_expression.hpp"
#include "guinsoodb/planner/expression/bound_case_expression.hpp"
#include "guinsoodb/optimizer/expression_rewriter.hpp"

namespace guinsoodb {

EmptyNeedleRemovalRule::EmptyNeedleRemovalRule(ExpressionRewriter &rewriter) : Rule(rewriter) {
	// match on a FunctionExpression that has a foldable ConstantExpression
	auto func = make_unique<FunctionExpressionMatcher>();
	func->matchers.push_back(make_unique<ExpressionMatcher>());
	func->matchers.push_back(make_unique<ExpressionMatcher>());
	func->policy = SetMatcher::Policy::SOME;

	unordered_set<string> functions = {"prefix", "contains", "suffix"};
	func->function = make_unique<ManyFunctionMatcher>(functions);
	root = move(func);
}

unique_ptr<Expression> EmptyNeedleRemovalRule::Apply(LogicalOperator &op, vector<Expression *> &bindings,
                                                     bool &changes_made) {
	auto root = (BoundFunctionExpression *)bindings[0];
	D_ASSERT(root->children.size() == 2);
	(void)root;
	auto prefix_expr = bindings[2];

	// the constant_expr is a scalar expression that we have to fold
	if (!prefix_expr->IsFoldable()) {
		return nullptr;
	}
	D_ASSERT(root->return_type.id() == LogicalTypeId::BOOLEAN);

	auto prefix_value = ExpressionExecutor::EvaluateScalar(*prefix_expr);

	if (prefix_value.is_null) {
		return make_unique<BoundConstantExpression>(Value(LogicalType::BOOLEAN));
	}

	D_ASSERT(prefix_value.type() == prefix_expr->return_type);
	auto needle_string = prefix_value.str_value;

	// PREFIX('xyz', '') is TRUE
	// PREFIX(NULL, '') is NULL
	// so rewrite PREFIX(x, '') to TRUE_OR_NULL(x)
	if (needle_string.empty()) {
		return ExpressionRewriter::ConstantOrNull(move(root->children[0]), Value::BOOLEAN(true));
	}
	return nullptr;
}

} // namespace guinsoodb

#include "guinsoodb/optimizer/optimizer.hpp"

#include "guinsoodb/execution/expression_executor.hpp"
#include "guinsoodb/main/client_context.hpp"
#include "guinsoodb/optimizer/column_lifetime_optimizer.hpp"
#include "guinsoodb/optimizer/common_aggregate_optimizer.hpp"
#include "guinsoodb/optimizer/cse_optimizer.hpp"
#include "guinsoodb/optimizer/deliminator.hpp"
#include "guinsoodb/optimizer/expression_heuristics.hpp"
#include "guinsoodb/optimizer/filter_pullup.hpp"
#include "guinsoodb/optimizer/filter_pushdown.hpp"
#include "guinsoodb/optimizer/in_clause_rewriter.hpp"
#include "guinsoodb/optimizer/join_order_optimizer.hpp"
#include "guinsoodb/optimizer/regex_range_filter.hpp"
#include "guinsoodb/optimizer/remove_unused_columns.hpp"
#include "guinsoodb/optimizer/rule/list.hpp"
#include "guinsoodb/optimizer/statistics_propagator.hpp"
#include "guinsoodb/optimizer/topn_optimizer.hpp"
#include "guinsoodb/planner/binder.hpp"

#include "guinsoodb/optimizer/rule/in_clause_simplification.hpp"

namespace guinsoodb {

Optimizer::Optimizer(Binder &binder, ClientContext &context) : context(context), binder(binder), rewriter(context) {
	rewriter.rules.push_back(make_unique<ConstantFoldingRule>(rewriter));
	rewriter.rules.push_back(make_unique<DistributivityRule>(rewriter));
	rewriter.rules.push_back(make_unique<ArithmeticSimplificationRule>(rewriter));
	rewriter.rules.push_back(make_unique<CaseSimplificationRule>(rewriter));
	rewriter.rules.push_back(make_unique<ConjunctionSimplificationRule>(rewriter));
	rewriter.rules.push_back(make_unique<DatePartSimplificationRule>(rewriter));
	rewriter.rules.push_back(make_unique<ComparisonSimplificationRule>(rewriter));
	rewriter.rules.push_back(make_unique<InClauseSimplificationRule>(rewriter));
	rewriter.rules.push_back(make_unique<MoveConstantsRule>(rewriter));
	rewriter.rules.push_back(make_unique<LikeOptimizationRule>(rewriter));
	rewriter.rules.push_back(make_unique<EmptyNeedleRemovalRule>(rewriter));

#ifdef DEBUG
	for (auto &rule : rewriter.rules) {
		// root not defined in rule
		D_ASSERT(rule->root);
	}
#endif
}

unique_ptr<LogicalOperator> Optimizer::Optimize(unique_ptr<LogicalOperator> plan) {
	// first we perform expression rewrites using the ExpressionRewriter
	// this does not change the logical plan structure, but only simplifies the expression trees
	context.profiler.StartPhase("expression_rewriter");
	rewriter.VisitOperator(*plan);
	context.profiler.EndPhase();

	// perform filter pullup
	context.profiler.StartPhase("filter_pullup");
	FilterPullup filter_pullup;
	plan = filter_pullup.Rewrite(move(plan));
	context.profiler.EndPhase();

	// perform filter pushdown
	context.profiler.StartPhase("filter_pushdown");
	FilterPushdown filter_pushdown(*this);
	plan = filter_pushdown.Rewrite(move(plan));
	context.profiler.EndPhase();

	context.profiler.StartPhase("regex_range");
	RegexRangeFilter regex_opt;
	plan = regex_opt.Rewrite(move(plan));
	context.profiler.EndPhase();

	context.profiler.StartPhase("in_clause");
	InClauseRewriter rewriter(*this);
	plan = rewriter.Rewrite(move(plan));
	context.profiler.EndPhase();

	// then we perform the join ordering optimization
	// this also rewrites cross products + filters into joins and performs filter pushdowns
	context.profiler.StartPhase("join_order");
	JoinOrderOptimizer optimizer(context);
	plan = optimizer.Optimize(move(plan));
	context.profiler.EndPhase();

	// removes any redundant DelimGets/DelimJoins
	context.profiler.StartPhase("deliminator");
	Deliminator deliminator;
	plan = deliminator.Optimize(move(plan));
	context.profiler.EndPhase();

	context.profiler.StartPhase("unused_columns");
	RemoveUnusedColumns unused(binder, context, true);
	unused.VisitOperator(*plan);
	context.profiler.EndPhase();

	// perform statistics propagation
	context.profiler.StartPhase("statistics_propagation");
	StatisticsPropagator propagator(context);
	propagator.PropagateStatistics(plan);
	context.profiler.EndPhase();

	// then we extract common subexpressions inside the different operators
	context.profiler.StartPhase("common_subexpressions");
	CommonSubExpressionOptimizer cse_optimizer(binder);
	cse_optimizer.VisitOperator(*plan);
	context.profiler.EndPhase();

	context.profiler.StartPhase("common_aggregate");
	CommonAggregateOptimizer common_aggregate;
	common_aggregate.VisitOperator(*plan);
	context.profiler.EndPhase();

	context.profiler.StartPhase("column_lifetime");
	ColumnLifetimeAnalyzer column_lifetime(true);
	column_lifetime.VisitOperator(*plan);
	context.profiler.EndPhase();

	// transform ORDER BY + LIMIT to TopN
	context.profiler.StartPhase("top_n");
	TopN topn;
	plan = topn.Optimize(move(plan));
	context.profiler.EndPhase();

	// apply simple expression heuristics to get an initial reordering
	context.profiler.StartPhase("reorder_filter");
	ExpressionHeuristics expression_heuristics(*this);
	plan = expression_heuristics.Rewrite(move(plan));
	context.profiler.EndPhase();

	return plan;
}

} // namespace guinsoodb

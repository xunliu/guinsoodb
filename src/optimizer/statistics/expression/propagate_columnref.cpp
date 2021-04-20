#include "guinsoodb/optimizer/statistics_propagator.hpp"
#include "guinsoodb/planner/expression/bound_columnref_expression.hpp"

namespace guinsoodb {

unique_ptr<BaseStatistics> StatisticsPropagator::PropagateExpression(BoundColumnRefExpression &colref,
                                                                     unique_ptr<Expression> *expr_ptr) {
	auto stats = statistics_map.find(colref.binding);
	if (stats == statistics_map.end()) {
		return nullptr;
	}
	return stats->second->Copy();
}

} // namespace guinsoodb

#include "guinsoodb/optimizer/statistics_propagator.hpp"
#include "guinsoodb/planner/operator/logical_projection.hpp"

namespace guinsoodb {

unique_ptr<NodeStatistics> StatisticsPropagator::PropagateStatistics(LogicalProjection &proj,
                                                                     unique_ptr<LogicalOperator> *node_ptr) {
	// first propagate to the child
	node_stats = PropagateStatistics(proj.children[0]);

	// then propagate to each of the expressions
	for (idx_t i = 0; i < proj.expressions.size(); i++) {
		auto stats = PropagateExpression(proj.expressions[i]);
		if (stats) {
			ColumnBinding binding(proj.table_index, i);
			statistics_map.insert(make_pair(binding, move(stats)));
		}
	}
	return move(node_stats);
}

} // namespace guinsoodb

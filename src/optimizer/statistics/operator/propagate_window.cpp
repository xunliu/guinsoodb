#include "guinsoodb/optimizer/statistics_propagator.hpp"
#include "guinsoodb/planner/operator/logical_window.hpp"

namespace guinsoodb {

unique_ptr<NodeStatistics> StatisticsPropagator::PropagateStatistics(LogicalWindow &order,
                                                                     unique_ptr<LogicalOperator> *node_ptr) {
	// propagate statistics in the child node
	return PropagateStatistics(order.children[0]);
}

} // namespace guinsoodb

#include "guinsoodb/optimizer/statistics_propagator.hpp"
#include "guinsoodb/planner/operator/logical_order.hpp"

namespace guinsoodb {

unique_ptr<NodeStatistics> StatisticsPropagator::PropagateStatistics(LogicalOrder &order,
                                                                     unique_ptr<LogicalOperator> *node_ptr) {
	// propagate statistics in the child node
	return PropagateStatistics(order.children[0]);
}

} // namespace guinsoodb

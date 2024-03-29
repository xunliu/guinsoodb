#include "guinsoodb/optimizer/statistics_propagator.hpp"
#include "guinsoodb/planner/operator/logical_limit.hpp"

namespace guinsoodb {

unique_ptr<NodeStatistics> StatisticsPropagator::PropagateStatistics(LogicalLimit &limit,
                                                                     unique_ptr<LogicalOperator> *node_ptr) {
	// propagate statistics in the child node
	PropagateStatistics(limit.children[0]);
	// return the node stats, with as expected cardinality the amount specified in the limit
	return make_unique<NodeStatistics>(limit.limit_val, limit.limit_val);
}

} // namespace guinsoodb

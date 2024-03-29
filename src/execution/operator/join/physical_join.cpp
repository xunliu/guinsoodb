#include "guinsoodb/execution/operator/join/physical_join.hpp"

namespace guinsoodb {

PhysicalJoin::PhysicalJoin(LogicalOperator &op, PhysicalOperatorType type, JoinType join_type,
                           idx_t estimated_cardinality)
    : PhysicalSink(type, op.types, estimated_cardinality), join_type(join_type) {
}

} // namespace guinsoodb

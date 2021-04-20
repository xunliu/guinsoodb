#include "guinsoodb/execution/operator/scan/physical_chunk_scan.hpp"
#include "guinsoodb/execution/physical_plan_generator.hpp"
#include "guinsoodb/planner/operator/logical_delim_get.hpp"

namespace guinsoodb {

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalDelimGet &op) {
	D_ASSERT(op.children.empty());

	// create a PhysicalChunkScan without an owned_collection, the collection will be added later
	auto chunk_scan =
	    make_unique<PhysicalChunkScan>(op.types, PhysicalOperatorType::DELIM_SCAN, op.estimated_cardinality);
	return move(chunk_scan);
}

} // namespace guinsoodb

#include "guinsoodb/execution/operator/scan/physical_chunk_scan.hpp"
#include "guinsoodb/execution/physical_plan_generator.hpp"
#include "guinsoodb/planner/operator/logical_chunk_get.hpp"

namespace guinsoodb {

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalChunkGet &op) {
	D_ASSERT(op.children.size() == 0);
	D_ASSERT(op.collection);

	// create a PhysicalChunkScan pointing towards the owned collection
	auto chunk_scan =
	    make_unique<PhysicalChunkScan>(op.types, PhysicalOperatorType::CHUNK_SCAN, op.estimated_cardinality);
	chunk_scan->owned_collection = move(op.collection);
	chunk_scan->collection = chunk_scan->owned_collection.get();
	return move(chunk_scan);
}

} // namespace guinsoodb

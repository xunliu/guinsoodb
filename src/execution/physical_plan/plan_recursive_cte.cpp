#include "guinsoodb/execution/operator/set/physical_recursive_cte.hpp"
#include "guinsoodb/execution/operator/scan/physical_chunk_scan.hpp"
#include "guinsoodb/execution/physical_plan_generator.hpp"
#include "guinsoodb/planner/expression/bound_reference_expression.hpp"
#include "guinsoodb/planner/operator/logical_recursive_cte.hpp"
#include "guinsoodb/planner/operator/logical_cteref.hpp"

namespace guinsoodb {

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalRecursiveCTE &op) {
	D_ASSERT(op.children.size() == 2);

	// Create the working_table that the PhysicalRecursiveCTE will use for evaluation.
	auto working_table = std::make_shared<ChunkCollection>();

	// Add the ChunkCollection to the context of this PhysicalPlanGenerator
	rec_ctes[op.table_index] = working_table;

	auto left = CreatePlan(*op.children[0]);
	auto right = CreatePlan(*op.children[1]);

	auto cte =
	    make_unique<PhysicalRecursiveCTE>(op.types, op.union_all, move(left), move(right), op.estimated_cardinality);
	cte->working_table = working_table;

	return move(cte);
}

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalCTERef &op) {
	D_ASSERT(op.children.empty());

	auto chunk_scan =
	    make_unique<PhysicalChunkScan>(op.types, PhysicalOperatorType::RECURSIVE_CTE_SCAN, op.estimated_cardinality);

	// CreatePlan of a LogicalRecursiveCTE must have happened before.
	auto cte = rec_ctes.find(op.cte_index);
	if (cte == rec_ctes.end()) {
		throw Exception("Referenced recursive CTE does not exist.");
	}
	chunk_scan->collection = cte->second.get();
	return move(chunk_scan);
}

} // namespace guinsoodb

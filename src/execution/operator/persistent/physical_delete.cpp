#include "guinsoodb/execution/operator/persistent/physical_delete.hpp"

#include "guinsoodb/execution/expression_executor.hpp"
#include "guinsoodb/storage/data_table.hpp"

#include <atomic>

namespace guinsoodb {

//===--------------------------------------------------------------------===//
// Sink
//===--------------------------------------------------------------------===//
class DeleteGlobalState : public GlobalOperatorState {
public:
	DeleteGlobalState() : deleted_count(0) {
	}

	std::atomic<idx_t> deleted_count;
};

void PhysicalDelete::Sink(ExecutionContext &context, GlobalOperatorState &state, LocalSinkState &lstate,
                          DataChunk &input) {
	auto &gstate = (DeleteGlobalState &)state;

	// delete data in the base table
	// the row ids are given to us as the last column of the child chunk
	table.Delete(tableref, context.client, input.data[row_id_index], input.size());
	gstate.deleted_count += input.size();
}

unique_ptr<GlobalOperatorState> PhysicalDelete::GetGlobalState(ClientContext &context) {
	return make_unique<DeleteGlobalState>();
}

//===--------------------------------------------------------------------===//
// GetChunkInternal
//===--------------------------------------------------------------------===//
void PhysicalDelete::GetChunkInternal(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state) {
	auto &gstate = (DeleteGlobalState &)*sink_state;

	chunk.SetCardinality(1);
	chunk.SetValue(0, 0, Value::BIGINT(gstate.deleted_count));

	state->finished = true;
}

} // namespace guinsoodb

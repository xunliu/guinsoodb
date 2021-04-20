#include "guinsoodb/execution/operator/scan/physical_empty_result.hpp"

namespace guinsoodb {

void PhysicalEmptyResult::GetChunkInternal(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state) {
	state->finished = true;
}

} // namespace guinsoodb

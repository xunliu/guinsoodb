#include "guinsoodb/execution/operator/helper/physical_vacuum.hpp"

namespace guinsoodb {

void PhysicalVacuum::GetChunkInternal(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state) {
	// NOP
	state->finished = true;
}

} // namespace guinsoodb

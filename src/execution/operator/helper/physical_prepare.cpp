#include "guinsoodb/execution/operator/helper/physical_prepare.hpp"
#include "guinsoodb/main/client_context.hpp"

namespace guinsoodb {

void PhysicalPrepare::GetChunkInternal(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state) {
	auto &client = context.client;

	// store the prepared statement in the context
	client.prepared_statements[name] = prepared;
	state->finished = true;
}

} // namespace guinsoodb

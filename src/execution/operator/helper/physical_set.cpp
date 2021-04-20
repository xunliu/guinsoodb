#include "guinsoodb/execution/operator/helper/physical_set.hpp"
#include "guinsoodb/main/database.hpp"
#include "guinsoodb/main/client_context.hpp"

namespace guinsoodb {

void PhysicalSet::GetChunkInternal(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state) {
	auto &db = context.client.db;
	db->config.set_variables[name] = value; // woop
	state->finished = true;
}

} // namespace guinsoodb

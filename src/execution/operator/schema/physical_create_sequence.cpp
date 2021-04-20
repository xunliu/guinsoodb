#include "guinsoodb/execution/operator/schema/physical_create_sequence.hpp"
#include "guinsoodb/catalog/catalog.hpp"

namespace guinsoodb {

void PhysicalCreateSequence::GetChunkInternal(ExecutionContext &context, DataChunk &chunk,
                                              PhysicalOperatorState *state) {
	Catalog::GetCatalog(context.client).CreateSequence(context.client, info.get());
	state->finished = true;
}

} // namespace guinsoodb

#include "guinsoodb/execution/operator/schema/physical_create_schema.hpp"
#include "guinsoodb/catalog/catalog.hpp"

namespace guinsoodb {

void PhysicalCreateSchema::GetChunkInternal(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state) {
	Catalog::GetCatalog(context.client).CreateSchema(context.client, info.get());
	state->finished = true;
}

} // namespace guinsoodb

#include "guinsoodb/execution/operator/schema/physical_create_function.hpp"

#include "guinsoodb/catalog/catalog.hpp"
#include "guinsoodb/catalog/catalog_entry/macro_catalog_entry.hpp"

namespace guinsoodb {

void PhysicalCreateFunction::GetChunkInternal(ExecutionContext &context, DataChunk &chunk,
                                              PhysicalOperatorState *state) {
	Catalog::GetCatalog(context.client).CreateFunction(context.client, info.get());
	state->finished = true;
}

} // namespace guinsoodb

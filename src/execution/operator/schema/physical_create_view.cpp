#include "guinsoodb/execution/operator/schema/physical_create_view.hpp"
#include "guinsoodb/catalog/catalog.hpp"

namespace guinsoodb {

void PhysicalCreateView::GetChunkInternal(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state) {
	Catalog::GetCatalog(context.client).CreateView(context.client, info.get());
	state->finished = true;
}

} // namespace guinsoodb

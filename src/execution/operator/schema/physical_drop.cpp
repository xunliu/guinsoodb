#include "guinsoodb/execution/operator/schema/physical_drop.hpp"
#include "guinsoodb/main/client_context.hpp"

namespace guinsoodb {

void PhysicalDrop::GetChunkInternal(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state) {
	switch (info->type) {
	case CatalogType::PREPARED_STATEMENT: {
		// DEALLOCATE silently ignores errors
		auto &statements = context.client.prepared_statements;
		if (statements.find(info->name) != statements.end()) {
			statements.erase(info->name);
		}
		break;
	}
	default:
		Catalog::GetCatalog(context.client).DropEntry(context.client, info.get());
		break;
	}
	state->finished = true;
}

} // namespace guinsoodb

#include "guinsoodb/execution/operator/schema/physical_alter.hpp"
#include "guinsoodb/parser/parsed_data/alter_table_info.hpp"
#include "guinsoodb/catalog/catalog.hpp"

namespace guinsoodb {

void PhysicalAlter::GetChunkInternal(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state) {
	auto &catalog = Catalog::GetCatalog(context.client);
	catalog.Alter(context.client, info.get());
	state->finished = true;
}

} // namespace guinsoodb

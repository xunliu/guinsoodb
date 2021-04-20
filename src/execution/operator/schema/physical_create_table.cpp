#include "guinsoodb/execution/operator/schema/physical_create_table.hpp"

#include "guinsoodb/catalog/catalog.hpp"
#include "guinsoodb/catalog/catalog_entry/table_catalog_entry.hpp"
#include "guinsoodb/execution/expression_executor.hpp"
#include "guinsoodb/storage/data_table.hpp"

namespace guinsoodb {

PhysicalCreateTable::PhysicalCreateTable(LogicalOperator &op, SchemaCatalogEntry *schema,
                                         unique_ptr<BoundCreateTableInfo> info, idx_t estimated_cardinality)
    : PhysicalOperator(PhysicalOperatorType::CREATE_TABLE, op.types, estimated_cardinality), schema(schema),
      info(move(info)) {
}

void PhysicalCreateTable::GetChunkInternal(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state) {
	auto &catalog = Catalog::GetCatalog(context.client);
	catalog.CreateTable(context.client, schema, info.get());
	state->finished = true;
}

} // namespace guinsoodb

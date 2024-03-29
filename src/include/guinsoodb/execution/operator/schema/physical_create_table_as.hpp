//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/execution/operator/schema/physical_create_table_as.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/execution/physical_sink.hpp"
#include "guinsoodb/planner/parsed_data/bound_create_table_info.hpp"

namespace guinsoodb {

//! Physically CREATE TABLE AS statement
class PhysicalCreateTableAs : public PhysicalSink {
public:
	PhysicalCreateTableAs(LogicalOperator &op, SchemaCatalogEntry *schema, unique_ptr<BoundCreateTableInfo> info,
	                      idx_t estimated_cardinality);

	//! Schema to insert to
	SchemaCatalogEntry *schema;
	//! Table name to create
	unique_ptr<BoundCreateTableInfo> info;

public:
	unique_ptr<GlobalOperatorState> GetGlobalState(ClientContext &context) override;

	void Sink(ExecutionContext &context, GlobalOperatorState &state, LocalSinkState &lstate, DataChunk &input) override;

	void GetChunkInternal(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state) override;
};
} // namespace guinsoodb

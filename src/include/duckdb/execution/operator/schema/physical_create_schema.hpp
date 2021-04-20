//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/execution/operator/schema/physical_create_schema.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/execution/physical_operator.hpp"
#include "guinsoodb/parser/parsed_data/create_schema_info.hpp"

namespace guinsoodb {

//! PhysicalCreateSchema represents a CREATE SCHEMA command
class PhysicalCreateSchema : public PhysicalOperator {
public:
	explicit PhysicalCreateSchema(unique_ptr<CreateSchemaInfo> info, idx_t estimated_cardinality)
	    : PhysicalOperator(PhysicalOperatorType::CREATE_SCHEMA, {LogicalType::BIGINT}, estimated_cardinality),
	      info(move(info)) {
	}

	unique_ptr<CreateSchemaInfo> info;

public:
	void GetChunkInternal(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state) override;
};

} // namespace guinsoodb

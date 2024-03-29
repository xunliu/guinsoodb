//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/execution/operator/schema/physical_create_sequence.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/execution/physical_operator.hpp"
#include "guinsoodb/parser/parsed_data/create_sequence_info.hpp"

namespace guinsoodb {

//! PhysicalCreateSequence represents a CREATE SEQUENCE command
class PhysicalCreateSequence : public PhysicalOperator {
public:
	explicit PhysicalCreateSequence(unique_ptr<CreateSequenceInfo> info, idx_t estimated_cardinality)
	    : PhysicalOperator(PhysicalOperatorType::CREATE_SEQUENCE, {LogicalType::BIGINT}, estimated_cardinality),
	      info(move(info)) {
	}

	unique_ptr<CreateSequenceInfo> info;

public:
	void GetChunkInternal(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state) override;
};

} // namespace guinsoodb

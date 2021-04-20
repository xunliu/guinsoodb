//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/execution/operator/schema/physical_create_view.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/execution/physical_operator.hpp"
#include "guinsoodb/parser/parsed_data/create_view_info.hpp"

namespace guinsoodb {

//! PhysicalCreateView represents a CREATE VIEW command
class PhysicalCreateView : public PhysicalOperator {
public:
	explicit PhysicalCreateView(unique_ptr<CreateViewInfo> info, idx_t estimated_cardinality)
	    : PhysicalOperator(PhysicalOperatorType::CREATE_VIEW, {LogicalType::BIGINT}, estimated_cardinality),
	      info(move(info)) {
	}

	unique_ptr<CreateViewInfo> info;

public:
	void GetChunkInternal(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state) override;
};

} // namespace guinsoodb

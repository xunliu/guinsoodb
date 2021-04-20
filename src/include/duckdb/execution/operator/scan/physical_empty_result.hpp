//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/execution/operator/scan/physical_empty_result.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/execution/physical_operator.hpp"

namespace guinsoodb {

class PhysicalEmptyResult : public PhysicalOperator {
public:
	explicit PhysicalEmptyResult(vector<LogicalType> types, idx_t estimated_cardinality)
	    : PhysicalOperator(PhysicalOperatorType::EMPTY_RESULT, move(types), estimated_cardinality) {
	}

public:
	void GetChunkInternal(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state) override;
};
} // namespace guinsoodb

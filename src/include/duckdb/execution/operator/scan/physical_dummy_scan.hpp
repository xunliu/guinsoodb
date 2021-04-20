//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/execution/operator/scan/physical_dummy_scan.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/execution/physical_operator.hpp"

namespace guinsoodb {

class PhysicalDummyScan : public PhysicalOperator {
public:
	explicit PhysicalDummyScan(vector<LogicalType> types, idx_t estimated_cardinality)
	    : PhysicalOperator(PhysicalOperatorType::DUMMY_SCAN, move(types), estimated_cardinality) {
	}

public:
	void GetChunkInternal(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state) override;
};
} // namespace guinsoodb

//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/execution/operator/scan/physical_chunk_scan.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/types/chunk_collection.hpp"
#include "guinsoodb/execution/physical_operator.hpp"

namespace guinsoodb {

//! The PhysicalChunkCollectionScan scans a Chunk Collection
class PhysicalChunkScan : public PhysicalOperator {
public:
	PhysicalChunkScan(vector<LogicalType> types, PhysicalOperatorType op_type, idx_t estimated_cardinality)
	    : PhysicalOperator(op_type, move(types), estimated_cardinality), collection(nullptr) {
	}

	void GetChunkInternal(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state) override;
	unique_ptr<PhysicalOperatorState> GetOperatorState() override;

public:
	// the chunk collection to scan
	ChunkCollection *collection;
	//! Owned chunk collection, if any
	unique_ptr<ChunkCollection> owned_collection;
};

} // namespace guinsoodb

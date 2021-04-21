//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/execution/operator/projection/physical_unnest.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/types/chunk_collection.hpp"
#include "guinsoodb/execution/physical_operator.hpp"

namespace guinsoodb {

//! PhysicalWindow implements window functions
class PhysicalUnnest : public PhysicalOperator {
public:
	PhysicalUnnest(vector<LogicalType> types, vector<unique_ptr<Expression>> select_list, idx_t estimated_cardinality,
	               PhysicalOperatorType type = PhysicalOperatorType::UNNEST);

	void GetChunkInternal(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state) override;

	//! The projection list of the SELECT statement (that contains aggregates)
	vector<unique_ptr<Expression>> select_list;

public:
	unique_ptr<PhysicalOperatorState> GetOperatorState() override;
};

} // namespace guinsoodb

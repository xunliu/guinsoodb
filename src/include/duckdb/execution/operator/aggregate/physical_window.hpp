//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/execution/operator/aggregate/physical_window.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/types/chunk_collection.hpp"
#include "guinsoodb/execution/physical_sink.hpp"
#include "guinsoodb/parallel/pipeline.hpp"

namespace guinsoodb {

//! PhysicalWindow implements window functions
//! It assumes that all functions have a common partitioning and ordering
class PhysicalWindow : public PhysicalSink {
public:
	PhysicalWindow(vector<LogicalType> types, vector<unique_ptr<Expression>> select_list, idx_t estimated_cardinality,
	               PhysicalOperatorType type = PhysicalOperatorType::WINDOW);

	void GetChunkInternal(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state) override;
	unique_ptr<PhysicalOperatorState> GetOperatorState() override;

	// sink stuff
	void Sink(ExecutionContext &context, GlobalOperatorState &state, LocalSinkState &lstate, DataChunk &input) override;
	void Combine(ExecutionContext &context, GlobalOperatorState &state, LocalSinkState &lstate) override;
	void Finalize(Pipeline &pipeline, ClientContext &context, unique_ptr<GlobalOperatorState> gstate) override;

	unique_ptr<LocalSinkState> GetLocalSinkState(ExecutionContext &context) override;
	unique_ptr<GlobalOperatorState> GetGlobalState(ClientContext &context) override;

	idx_t MaxThreads(ClientContext &context);
	unique_ptr<ParallelState> GetParallelState();

	string ParamsToString() const override;

public:
	//! The projection list of the WINDOW statement (may contain aggregates)
	vector<unique_ptr<Expression>> select_list;
};

} // namespace guinsoodb
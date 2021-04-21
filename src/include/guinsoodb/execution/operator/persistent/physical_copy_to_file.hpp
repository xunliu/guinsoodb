//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/execution/operator/persistent/physical_copy_to_file.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/execution/physical_sink.hpp"
#include "guinsoodb/parser/parsed_data/copy_info.hpp"
#include "guinsoodb/function/copy_function.hpp"

namespace guinsoodb {

//! Copy the contents of a query into a table
class PhysicalCopyToFile : public PhysicalSink {
public:
	PhysicalCopyToFile(vector<LogicalType> types, CopyFunction function, unique_ptr<FunctionData> bind_data,
	                   idx_t estimated_cardinality)
	    : PhysicalSink(PhysicalOperatorType::COPY_TO_FILE, move(types), estimated_cardinality), function(function),
	      bind_data(move(bind_data)) {
	}

	CopyFunction function;
	unique_ptr<FunctionData> bind_data;

public:
	void GetChunkInternal(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state) override;

	void Sink(ExecutionContext &context, GlobalOperatorState &gstate, LocalSinkState &lstate,
	          DataChunk &input) override;
	void Combine(ExecutionContext &context, GlobalOperatorState &gstate, LocalSinkState &lstate) override;
	void Finalize(Pipeline &pipeline, ClientContext &context, unique_ptr<GlobalOperatorState> gstate) override;
	unique_ptr<LocalSinkState> GetLocalSinkState(ExecutionContext &context) override;
	unique_ptr<GlobalOperatorState> GetGlobalState(ClientContext &context) override;
};
} // namespace guinsoodb

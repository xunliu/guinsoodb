//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/execution/operator/helper/physical_reservoir_sample.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/execution/physical_sink.hpp"
#include "guinsoodb/parser/parsed_data/sample_options.hpp"

namespace guinsoodb {

//! PhysicalReservoirSample represents a sample taken using reservoir sampling, which is a blocking sampling method
class PhysicalReservoirSample : public PhysicalSink {
public:
	PhysicalReservoirSample(vector<LogicalType> types, unique_ptr<SampleOptions> options, idx_t estimated_cardinality)
	    : PhysicalSink(PhysicalOperatorType::RESERVOIR_SAMPLE, move(types), estimated_cardinality),
	      options(move(options)) {
	}

	unique_ptr<SampleOptions> options;

public:
	void Sink(ExecutionContext &context, GlobalOperatorState &state, LocalSinkState &lstate, DataChunk &input) override;
	unique_ptr<GlobalOperatorState> GetGlobalState(ClientContext &context) override;

	void GetChunkInternal(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state) override;
	unique_ptr<PhysicalOperatorState> GetOperatorState() override;

	string ParamsToString() const override;
};

} // namespace guinsoodb

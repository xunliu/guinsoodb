#include "guinsoodb/execution/operator/helper/physical_pragma.hpp"

namespace guinsoodb {

void PhysicalPragma::GetChunkInternal(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state) {
	auto &client = context.client;
	FunctionParameters parameters {info.parameters, info.named_parameters};
	function.function(client, parameters);
}

} // namespace guinsoodb

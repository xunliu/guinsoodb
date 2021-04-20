#include "guinsoodb/execution/physical_operator.hpp"

#include "guinsoodb/common/printer.hpp"
#include "guinsoodb/common/string_util.hpp"
#include "guinsoodb/execution/execution_context.hpp"
#include "guinsoodb/main/client_context.hpp"
#include "guinsoodb/parallel/thread_context.hpp"
#include "guinsoodb/common/tree_renderer.hpp"

namespace guinsoodb {

string PhysicalOperator::GetName() const {
	return PhysicalOperatorToString(type);
}

string PhysicalOperator::ToString() const {
	TreeRenderer renderer;
	return renderer.ToString(*this);
}

PhysicalOperatorState::PhysicalOperatorState(PhysicalOperator &op, PhysicalOperator *child) : finished(false) {
	op.InitializeChunk(initial_chunk);
	if (child) {
		child->InitializeChunkEmpty(child_chunk);
		child_state = child->GetOperatorState();
	}
}

void PhysicalOperator::GetChunk(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state) {
	if (context.client.interrupted) {
		throw InterruptException();
	}
	// reset the chunk back to its initial state
	chunk.Reference(state->initial_chunk);

	if (state->finished) {
		return;
	}

	// execute the operator
	context.thread.profiler.StartOperator(this);
	GetChunkInternal(context, chunk, state);
	context.thread.profiler.EndOperator(&chunk);

	chunk.Verify();
}

void PhysicalOperator::Print() {
	Printer::Print(ToString());
}

} // namespace guinsoodb

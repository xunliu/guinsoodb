//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/execution/expression_executor_state.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/common.hpp"
#include "guinsoodb/common/types/data_chunk.hpp"
#include "guinsoodb/common/cycle_counter.hpp"

namespace guinsoodb {
class Expression;
class ExpressionExecutor;
struct ExpressionExecutorState;

struct ExpressionState {
	ExpressionState(Expression &expr, ExpressionExecutorState &root);
	virtual ~ExpressionState() {
	}

	Expression &expr;
	ExpressionExecutorState &root;
	vector<unique_ptr<ExpressionState>> child_states;
	vector<LogicalType> types;
	DataChunk intermediate_chunk;
	string name;
	double time;
	CycleCounter profiler;

public:
	void AddChild(Expression *expr);
	void Finalize();
};

struct ExpressionExecutorState {
	unique_ptr<ExpressionState> root_state;
	ExpressionExecutor *executor;
};

} // namespace guinsoodb
//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/execution/executor.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/common.hpp"
#include "guinsoodb/common/mutex.hpp"
#include "guinsoodb/parallel/pipeline.hpp"
#include "guinsoodb/common/unordered_map.hpp"

#include <queue>

namespace guinsoodb {
class ClientContext;
class DataChunk;
class PhysicalOperator;
class PhysicalOperatorState;
class ThreadContext;
class Task;

struct ProducerToken;

class Executor {
	friend class Pipeline;
	friend class PipelineTask;

public:
	explicit Executor(ClientContext &context);
	~Executor();

	ClientContext &context;

public:
	void Initialize(PhysicalOperator *physical_plan);
	void BuildPipelines(PhysicalOperator *op, Pipeline *parent);

	void Reset();

	vector<LogicalType> GetTypes();

	unique_ptr<DataChunk> FetchChunk();

	//! Push a new error
	void PushError(const string &exception);

	//! Flush a thread context into the client context
	void Flush(ThreadContext &context);

	//! Returns the progress of the pipelines
	bool GetPipelinesProgress(int &current_progress);

private:
	PhysicalOperator *physical_plan;
	unique_ptr<PhysicalOperatorState> physical_state;

	mutex executor_lock;
	//! The pipelines of the current query
	vector<unique_ptr<Pipeline>> pipelines;
	//! The producer of this query
	unique_ptr<ProducerToken> producer;
	//! Exceptions that occurred during the execution of the current query
	vector<string> exceptions;

	//! The amount of completed pipelines of the query
	std::atomic<idx_t> completed_pipelines;
	//! The total amount of pipelines in the query
	idx_t total_pipelines;

	unordered_map<PhysicalOperator *, Pipeline *> delim_join_dependencies;
	PhysicalOperator *recursive_cte;
};
} // namespace guinsoodb

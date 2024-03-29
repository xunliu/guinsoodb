//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/main/query_profiler.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/common.hpp"
#include "guinsoodb/common/enums/profiler_format.hpp"
#include "guinsoodb/common/profiler.hpp"
#include "guinsoodb/common/string_util.hpp"
#include "guinsoodb/common/types/data_chunk.hpp"
#include "guinsoodb/common/unordered_map.hpp"
#include "guinsoodb/common/winapi.hpp"
#include "guinsoodb/execution/physical_operator.hpp"
#include "guinsoodb/execution/expression_executor_state.hpp"
#include <stack>
#include <unordered_map>
#include "guinsoodb/common/pair.hpp"
#include "guinsoodb/common/deque.hpp"

namespace guinsoodb {
class ExpressionExecutor;
class PhysicalOperator;
class SQLStatement;

struct ExpressionInformation {
	ExpressionInformation(string &name, double time) : name(name), time(time) {
	}
	void ExtractExpressionsRecursive(unique_ptr<ExpressionState> &state);
	vector<unique_ptr<ExpressionInformation>> children;
	bool hasfunction = false;
	string name;
	string function_name;
	uint64_t time = 0;
};

struct ExpressionExecutorInformation {
	explicit ExpressionExecutorInformation(ExpressionExecutor &executor);

	//! Count the number of time the executor called
	uint64_t total_count = 0;
	//! Count the number of time the executor called since last sampling
	uint64_t current_count = 0;
	//! Count the number of samples
	uint64_t sample_count = 0;
	//! Count the number of tuples in all samples
	uint64_t sample_tuples_count = 0;
	//! Count the number of tuples processed by this executor
	uint64_t tuples_count = 0;

	vector<unique_ptr<ExpressionInformation>> roots;
};

struct OperatorTimingInformation {
	double time = 0;
	idx_t elements = 0;
	bool has_executor = false;
	explicit OperatorTimingInformation(double time_ = 0, idx_t elements_ = 0) : time(time_), elements(elements_) {
	}

	//! A mapping of physical operators to recorded timings
	unique_ptr<ExpressionExecutorInformation> executors_info;
};

//! The OperatorProfiler measures timings of individual operators
class OperatorProfiler {
	friend class QueryProfiler;

public:
	GUINSOODB_API explicit OperatorProfiler(bool enabled);

	GUINSOODB_API void StartOperator(PhysicalOperator *phys_op);
	GUINSOODB_API void EndOperator(DataChunk *chunk);
	GUINSOODB_API void Flush(PhysicalOperator *phys_op, ExpressionExecutor *expression_executor);

	~OperatorProfiler() {
	}

private:
	void AddTiming(PhysicalOperator *op, double time, idx_t elements);

	//! Whether or not the profiler is enabled
	bool enabled;
	//! The timer used to time the execution time of the individual Physical Operators
	Profiler<system_clock> op;
	//! The stack of Physical Operators that are currently active
	std::stack<PhysicalOperator *> execution_stack;
	//! A mapping of physical operators to recorded timings
	unordered_map<PhysicalOperator *, OperatorTimingInformation> timings;
};

//! The QueryProfiler can be used to measure timings of queries
class QueryProfiler {
public:
	struct TreeNode {
		string name;
		string extra_info;
		OperatorTimingInformation info;
		vector<unique_ptr<TreeNode>> children;
		idx_t depth = 0;
	};

private:
	unique_ptr<TreeNode> CreateTree(PhysicalOperator *root, idx_t depth = 0);

	void Render(const TreeNode &node, std::ostream &str) const;

public:
	GUINSOODB_API QueryProfiler()
	    : automatic_print_format(ProfilerPrintFormat::NONE), enabled(false), detailed_enabled(false), running(false) {
	}

	GUINSOODB_API void Enable() {
		enabled = true;
		detailed_enabled = false;
	}

	GUINSOODB_API void DetailedEnable() {
		detailed_enabled = true;
	}

	GUINSOODB_API void Disable() {
		enabled = false;
	}

	GUINSOODB_API bool IsEnabled() {
		return enabled;
	}

	bool IsDetailedEnabled() const {
		return detailed_enabled;
	}

	GUINSOODB_API void StartQuery(string query);
	GUINSOODB_API void EndQuery();

	//! Adds the timings gathered by an OperatorProfiler to this query profiler
	GUINSOODB_API void Flush(OperatorProfiler &profiler);

	GUINSOODB_API void StartPhase(string phase);
	GUINSOODB_API void EndPhase();

	GUINSOODB_API void Initialize(PhysicalOperator *root);

	GUINSOODB_API string ToString(bool print_optimizer_output = false) const;
	GUINSOODB_API void ToStream(std::ostream &str, bool print_optimizer_output = false) const;
	GUINSOODB_API void Print();

	GUINSOODB_API string ToJSON() const;
	GUINSOODB_API void WriteToFile(const char *path, string &info) const;

	//! The format to automatically print query profiling information in (default: disabled)
	ProfilerPrintFormat automatic_print_format;
	//! The file to save query profiling information to, instead of printing it to the console (empty = print to
	//! console)
	string save_location;

	idx_t OperatorSize() {
		return tree_map.size();
	}

private:
	//! Whether or not query profiling is enabled
	bool enabled;
	//! Whether or not detailed query profiling is enabled
	bool detailed_enabled;
	//! Whether or not the query profiler is running
	bool running;

	bool query_requires_profiling;

	//! The root of the query tree
	unique_ptr<TreeNode> root;
	//! The query string
	string query;

	//! The timer used to time the execution time of the entire query
	Profiler<system_clock> main_query;
	//! A map of a Physical Operator pointer to a tree node
	unordered_map<PhysicalOperator *, TreeNode *> tree_map;

public:
	const unordered_map<PhysicalOperator *, TreeNode *> &GetTreeMap() const {
		return tree_map;
	}

private:
	//! The timer used to time the individual phases of the planning process
	Profiler<system_clock> phase_profiler;
	//! A mapping of the phase names to the timings
	using PhaseTimingStorage = unordered_map<string, double>;
	PhaseTimingStorage phase_timings;
	using PhaseTimingItem = PhaseTimingStorage::value_type;
	//! The stack of currently active phases
	vector<string> phase_stack;

private:
	vector<PhaseTimingItem> GetOrderedPhaseTimings() const;

	//! Check whether or not an operator type requires query profiling. If none of the ops in a query require profiling
	//! no profiling information is output.
	bool OperatorRequiresProfiling(PhysicalOperatorType op_type);
};

//! The QueryProfilerHistory can be used to access the profiler of previous queries
class QueryProfilerHistory {
private:
	//! Previous Query profilers
	deque<pair<transaction_t, QueryProfiler>> prev_profilers;
	//! Previous Query profilers size
	uint64_t prev_profilers_size = 20;

public:
	deque<pair<transaction_t, QueryProfiler>> &GetPrevProfilers() {
		return prev_profilers;
	}

	void SetPrevProfilersSize(uint64_t prevProfilersSize) {
		prev_profilers_size = prevProfilersSize;
	}
	uint64_t GetPrevProfilersSize() const {
		return prev_profilers_size;
	}

public:
	void SetProfilerHistorySize(uint64_t size) {
		this->prev_profilers_size = size;
	}
};
} // namespace guinsoodb
//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parallel/task_context.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/common.hpp"
#include "guinsoodb/common/unordered_map.hpp"
#include "guinsoodb/parallel/parallel_state.hpp"

namespace guinsoodb {
class PhysicalOperator;

//! TaskContext holds task specific information relating to the excution
class TaskContext {
public:
	TaskContext() {
	}

	//! Per-operator task info
	unordered_map<PhysicalOperator *, ParallelState *> task_info;
};

} // namespace guinsoodb

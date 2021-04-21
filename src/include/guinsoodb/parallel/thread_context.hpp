//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parallel/thread_context.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/main/query_profiler.hpp"

namespace guinsoodb {
class ClientContext;

//! The ThreadContext holds thread-local info for parallel usage
class ThreadContext {
public:
	explicit ThreadContext(ClientContext &context);

	//! The operator profiler for the individual thread context
	OperatorProfiler profiler;
};

} // namespace guinsoodb

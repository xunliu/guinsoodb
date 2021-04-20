//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parallel/task.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

namespace guinsoodb {

class Task {
public:
	virtual ~Task() {
	}

	//! Execute the task
	virtual void Execute() = 0;
};

} // namespace guinsoodb

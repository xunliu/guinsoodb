//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/common/cycle_counter.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/helper.hpp"
#include "guinsoodb/common/chrono.hpp"

namespace guinsoodb {

//! The cycle counter can be used to measure elapsed cycles
class CycleCounter {
public:
	//! Starts the timer
	void Start() {
		finished = false;
		start = Tick();
	}
	//! Finishes timing
	void End() {
		end = Tick();
		finished = true;
	}

	uint64_t Elapsed() const {
		return end - start;
	}

private:
	uint64_t Tick() const;
	uint64_t start;
	uint64_t end;
	bool finished = false;
};

} // namespace guinsoodb

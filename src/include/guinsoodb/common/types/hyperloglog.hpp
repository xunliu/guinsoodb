//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/common/types/hyperloglog.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/types/vector.hpp"

namespace guinsoodb {

//! The HyperLogLog class holds a HyperLogLog counter for approximate cardinality counting
class HyperLogLog {
public:
	HyperLogLog();
	~HyperLogLog();
	// implicit copying of HyperLogLog is not allowed
	HyperLogLog(const HyperLogLog &) = delete;

	//! Adds an element of the specified size to the HyperLogLog counter
	void Add(data_ptr_t element, idx_t size);
	//! Return the count of this HyperLogLog counter
	idx_t Count();
	//! Merge this HyperLogLog counter with another counter to create a new one
	unique_ptr<HyperLogLog> Merge(HyperLogLog &other);
	HyperLogLog *MergePointer(HyperLogLog &other);
	//! Merge a set of HyperLogLogs to create one big one
	static unique_ptr<HyperLogLog> Merge(HyperLogLog logs[], idx_t count);

private:
	HyperLogLog(void *hll);

	void *hll;
};
} // namespace guinsoodb
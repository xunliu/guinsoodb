//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/storage/data_pointer.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/common.hpp"
#include "guinsoodb/storage/statistics/base_statistics.hpp"
#include "guinsoodb/storage/storage_info.hpp"

namespace guinsoodb {

class DataPointer {
public:
	uint64_t row_start;
	uint64_t tuple_count;
	block_id_t block_id;
	uint32_t offset;
	//! Type-specific statistics of the segment
	unique_ptr<BaseStatistics> statistics;
};

} // namespace guinsoodb

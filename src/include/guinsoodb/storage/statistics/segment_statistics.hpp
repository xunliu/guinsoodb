//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/storage/statistics/segment_statistics.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/common.hpp"
#include "guinsoodb/common/types.hpp"
#include "guinsoodb/storage/statistics/base_statistics.hpp"

namespace guinsoodb {
struct TableFilter;

class SegmentStatistics {
public:
	SegmentStatistics(LogicalType type, idx_t type_size);
	SegmentStatistics(LogicalType type, idx_t type_size, unique_ptr<BaseStatistics> statistics);

	LogicalType type;
	idx_t type_size;

	//! Type-specific statistics of the segment
	unique_ptr<BaseStatistics> statistics;

public:
	bool CheckZonemap(TableFilter &filter);
	void Reset();
};

} // namespace guinsoodb

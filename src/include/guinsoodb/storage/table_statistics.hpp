//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/storage/table_statistics.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/common.hpp"
#include "guinsoodb/common/types/data_chunk.hpp"

namespace guinsoodb {

struct TableStatistics {
	idx_t estimated_cardinality;
};

} // namespace guinsoodb

//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/transaction/delete_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/constants.hpp"

namespace guinsoodb {
class ChunkVectorInfo;
class DataTable;

struct DeleteInfo {
	DataTable *table;
	ChunkVectorInfo *vinfo;
	idx_t count;
	idx_t base_row;
	row_t rows[1];
};

} // namespace guinsoodb

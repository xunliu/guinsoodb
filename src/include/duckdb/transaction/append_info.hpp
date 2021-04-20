//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/transaction/append_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/constants.hpp"

namespace guinsoodb {
class DataTable;

struct AppendInfo {
	DataTable *table;
	idx_t start_row;
	idx_t count;
};

} // namespace guinsoodb

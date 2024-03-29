//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/common/enums/profiler_format.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/constants.hpp"

namespace guinsoodb {

enum class ProfilerPrintFormat : uint8_t { NONE, QUERY_TREE, JSON, QUERY_TREE_OPTIMIZER };

} // namespace guinsoodb

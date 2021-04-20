//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/common/enums/output_type.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/constants.hpp"

namespace guinsoodb {

enum class ExplainOutputType : uint8_t { ALL = 0, OPTIMIZED_ONLY = 1, PHYSICAL_ONLY = 2 };

} // namespace guinsoodb

//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/common/enums/set_operation_type.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/constants.hpp"

namespace guinsoodb {

enum class SetOperationType : uint8_t { NONE = 0, UNION = 1, EXCEPT = 2, INTERSECT = 3 };
}

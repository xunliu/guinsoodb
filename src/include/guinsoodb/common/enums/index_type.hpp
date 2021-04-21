//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/common/enums/index_type.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/constants.hpp"

namespace guinsoodb {

//===--------------------------------------------------------------------===//
// Index Types
//===--------------------------------------------------------------------===//
enum class IndexType {
	INVALID = 0, // invalid index type
	ART = 1      // Adaptive Radix Tree
};

} // namespace guinsoodb

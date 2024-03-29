//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/common/enums/undo_flags.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/constants.hpp"

namespace guinsoodb {

enum class UndoFlags : uint32_t { // far to big but aligned (TM)
	EMPTY_ENTRY = 0,
	CATALOG_ENTRY = 1,
	INSERT_TUPLE = 2,
	DELETE_TUPLE = 3,
	UPDATE_TUPLE = 4
};

} // namespace guinsoodb

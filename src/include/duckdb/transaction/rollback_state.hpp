//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/transaction/rollback_state.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/transaction/undo_buffer.hpp"

namespace guinsoodb {
class DataChunk;
class DataTable;
class WriteAheadLog;

class RollbackState {
public:
	RollbackState() {
	}

public:
	void RollbackEntry(UndoFlags type, data_ptr_t data);
};

} // namespace guinsoodb

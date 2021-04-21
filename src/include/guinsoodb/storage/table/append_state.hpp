//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/storage/table/append_state.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/common.hpp"
#include "guinsoodb/storage/storage_lock.hpp"
#include "guinsoodb/storage/buffer/buffer_handle.hpp"

namespace guinsoodb {
class UpdateSegment;
class TransientSegment;
class ValiditySegment;

struct ColumnAppendState {
	//! The current segment of the append
	TransientSegment *current;
	//! The update segment to append to
	UpdateSegment *updates;
	//! Child append states
	vector<ColumnAppendState> child_appends;
	//! The write lock that is held by the append
	unique_ptr<StorageLockKey> lock;
};

struct IndexLock {
	std::unique_lock<std::mutex> index_lock;
};

struct TableAppendState {
	std::unique_lock<std::mutex> append_lock;
	unique_ptr<IndexLock[]> index_locks;
	unique_ptr<ColumnAppendState[]> states;
	row_t row_start;
	row_t current_row;
};

} // namespace guinsoodb

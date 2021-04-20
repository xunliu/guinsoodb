//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/transaction/commit_state.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/transaction/undo_buffer.hpp"
#include "guinsoodb/common/vector_size.hpp"

namespace guinsoodb {
class CatalogEntry;
class DataChunk;
class WriteAheadLog;

struct DataTableInfo;
struct DeleteInfo;
struct UpdateInfo;

class CommitState {
public:
	explicit CommitState(transaction_t commit_id, WriteAheadLog *log = nullptr);

	WriteAheadLog *log;
	transaction_t commit_id;
	UndoFlags current_op;

	DataTableInfo *current_table_info;
	idx_t row_identifiers[STANDARD_VECTOR_SIZE];

	unique_ptr<DataChunk> delete_chunk;
	unique_ptr<DataChunk> update_chunk;

public:
	template <bool HAS_LOG>
	void CommitEntry(UndoFlags type, data_ptr_t data);
	void RevertCommit(UndoFlags type, data_ptr_t data);

private:
	void SwitchTable(DataTableInfo *table, UndoFlags new_op);

	void WriteCatalogEntry(CatalogEntry *entry, data_ptr_t extra_data);
	void WriteDelete(DeleteInfo *info);
	void WriteUpdate(UpdateInfo *info);

	void AppendRowId(row_t rowid);
};

} // namespace guinsoodb
#include "guinsoodb/transaction/rollback_state.hpp"
#include "guinsoodb/transaction/append_info.hpp"
#include "guinsoodb/transaction/delete_info.hpp"
#include "guinsoodb/transaction/update_info.hpp"

#include "guinsoodb/storage/uncompressed_segment.hpp"
#include "guinsoodb/storage/table/chunk_info.hpp"

#include "guinsoodb/catalog/catalog_entry.hpp"
#include "guinsoodb/catalog/catalog_set.hpp"
#include "guinsoodb/storage/data_table.hpp"
#include "guinsoodb/storage/table/update_segment.hpp"

namespace guinsoodb {

void RollbackState::RollbackEntry(UndoFlags type, data_ptr_t data) {
	switch (type) {
	case UndoFlags::CATALOG_ENTRY: {
		// undo this catalog entry
		auto catalog_entry = Load<CatalogEntry *>(data);
		D_ASSERT(catalog_entry->set);
		catalog_entry->set->Undo(catalog_entry);
		break;
	}
	case UndoFlags::INSERT_TUPLE: {
		auto info = (AppendInfo *)data;
		// revert the append in the base table
		info->table->RevertAppend(info->start_row, info->count);
		break;
	}
	case UndoFlags::DELETE_TUPLE: {
		auto info = (DeleteInfo *)data;
		// reset the deleted flag on rollback
		info->vinfo->CommitDelete(NOT_DELETED_ID, info->rows, info->count);
		break;
	}
	case UndoFlags::UPDATE_TUPLE: {
		auto info = (UpdateInfo *)data;
		info->segment->RollbackUpdate(info);
		break;
	}
	default:
		D_ASSERT(type == UndoFlags::EMPTY_ENTRY);
		break;
	}
}

} // namespace guinsoodb

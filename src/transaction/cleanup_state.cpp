#include "guinsoodb/transaction/cleanup_state.hpp"
#include "guinsoodb/transaction/delete_info.hpp"
#include "guinsoodb/transaction/update_info.hpp"

#include "guinsoodb/storage/data_table.hpp"
#include "guinsoodb/storage/uncompressed_segment.hpp"

#include "guinsoodb/catalog/catalog.hpp"
#include "guinsoodb/catalog/dependency_manager.hpp"
#include "guinsoodb/storage/table/chunk_info.hpp"
#include "guinsoodb/storage/table/update_segment.hpp"

namespace guinsoodb {

CleanupState::CleanupState() : current_table(nullptr), count(0) {
}

CleanupState::~CleanupState() {
	Flush();
}

void CleanupState::CleanupEntry(UndoFlags type, data_ptr_t data) {
	switch (type) {
	case UndoFlags::CATALOG_ENTRY: {
		auto catalog_entry = Load<CatalogEntry *>(data);
		// destroy the backed up entry: it is no longer required
		D_ASSERT(catalog_entry->parent);
		if (catalog_entry->parent->type != CatalogType::UPDATED_ENTRY) {
			if (!catalog_entry->deleted) {
				// delete the entry from the dependency manager, if it is not deleted yet
				catalog_entry->catalog->dependency_manager->EraseObject(catalog_entry);
			}
			catalog_entry->parent->child = move(catalog_entry->child);
		}
		break;
	}
	case UndoFlags::DELETE_TUPLE: {
		auto info = (DeleteInfo *)data;
		CleanupDelete(info);
		break;
	}
	case UndoFlags::UPDATE_TUPLE: {
		auto info = (UpdateInfo *)data;
		CleanupUpdate(info);
		break;
	}
	default:
		break;
	}
}

void CleanupState::CleanupUpdate(UpdateInfo *info) {
	// remove the update info from the update chain
	// first obtain an exclusive lock on the segment
	info->segment->CleanupUpdate(info);
}

void CleanupState::CleanupDelete(DeleteInfo *info) {
	auto version_table = info->table;
	version_table->info->cardinality -= info->count;
	if (version_table->info->indexes.empty()) {
		// this table has no indexes: no cleanup to be done
		return;
	}
	if (current_table != version_table) {
		// table for this entry differs from previous table: flush and switch to the new table
		Flush();
		current_table = version_table;
	}
	for (idx_t i = 0; i < info->count; i++) {
		if (count == STANDARD_VECTOR_SIZE) {
			Flush();
		}
		row_numbers[count++] = info->vinfo->start + info->rows[i];
	}
}

void CleanupState::Flush() {
	if (count == 0) {
		return;
	}

	// set up the row identifiers vector
	Vector row_identifiers(LOGICAL_ROW_TYPE, (data_ptr_t)row_numbers);

	// delete the tuples from all the indexes
	current_table->RemoveFromIndexes(row_identifiers, count);

	count = 0;
}

} // namespace guinsoodb

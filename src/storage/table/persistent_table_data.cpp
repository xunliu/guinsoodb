#include "guinsoodb/storage/table/persistent_segment.hpp"
#include "guinsoodb/storage/table/persistent_table_data.hpp"
#include "guinsoodb/storage/statistics/base_statistics.hpp"

namespace guinsoodb {

PersistentTableData::PersistentTableData(idx_t column_count) {
	column_data.resize(column_count);
}

PersistentTableData::~PersistentTableData() {
}

PersistentColumnData::~PersistentColumnData() {
}

} // namespace guinsoodb

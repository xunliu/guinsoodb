//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/storage/table/persistent_table_data.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/constants.hpp"
#include "guinsoodb/common/vector.hpp"
#include "guinsoodb/storage/table/segment_tree.hpp"

namespace guinsoodb {
class BaseStatistics;
class PersistentSegment;

class PersistentColumnData {
public:
	virtual ~PersistentColumnData();

	vector<unique_ptr<PersistentSegment>> segments;
	unique_ptr<BaseStatistics> stats;
	idx_t total_rows = 0;
};

class StandardPersistentColumnData : public PersistentColumnData {
public:
	unique_ptr<PersistentColumnData> validity;
};

class PersistentTableData {
public:
	explicit PersistentTableData(idx_t column_count);
	~PersistentTableData();

	vector<unique_ptr<PersistentColumnData>> column_data;
	shared_ptr<SegmentTree> versions;
};

} // namespace guinsoodb

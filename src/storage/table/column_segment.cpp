#include "guinsoodb/storage/table/column_segment.hpp"
#include "guinsoodb/common/limits.hpp"

#include <cstring>

namespace guinsoodb {

ColumnSegment::ColumnSegment(LogicalType type_p, ColumnSegmentType segment_type, idx_t start, idx_t count)
    : SegmentBase(start, count), type(move(type_p)), type_size(GetTypeIdSize(type.InternalType())),
      segment_type(segment_type), stats(type, type_size) {
}

ColumnSegment::ColumnSegment(LogicalType type_p, ColumnSegmentType segment_type, idx_t start, idx_t count,
                             unique_ptr<BaseStatistics> statistics)
    : SegmentBase(start, count), type(move(type_p)), type_size(GetTypeIdSize(type.InternalType())),
      segment_type(segment_type), stats(type, type_size, move(statistics)) {
}

} // namespace guinsoodb

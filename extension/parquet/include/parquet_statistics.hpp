#pragma once

#include "guinsoodb.hpp"
#ifndef GUINSOODB_AMALGAMATION
#include "guinsoodb/storage/statistics/base_statistics.hpp"
#endif
#include "parquet_types.h"

namespace guinsoodb {

using parquet::format::ColumnChunk;
using parquet::format::SchemaElement;

struct LogicalType;

unique_ptr<BaseStatistics> ParquetTransformColumnStatistics(const SchemaElement &s_ele, const LogicalType &type,
                                                            const ColumnChunk &column_chunk);

} // namespace guinsoodb

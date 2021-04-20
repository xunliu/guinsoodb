#include "parquet_statistics.hpp"
#include "parquet_timestamp.hpp"

#include "guinsoodb.hpp"
#ifndef DUCKDB_AMALGAMATION
#include "guinsoodb/common/types/value.hpp"
#include "guinsoodb/storage/statistics/string_statistics.hpp"
#include "guinsoodb/storage/statistics/numeric_statistics.hpp"
#endif

namespace guinsoodb {

using parquet::format::ConvertedType;
using parquet::format::Type;

template <Value (*FUNC)(const_data_ptr_t input)>
static unique_ptr<BaseStatistics> TemplatedGetNumericStats(const LogicalType &type,
                                                           const parquet::format::Statistics &parquet_stats) {
	auto stats = make_unique<NumericStatistics>(type);

	// for reasons unknown to science, Parquet defines *both* `min` and `min_value` as well as `max` and
	// `max_value`. All are optional. such elegance.
	if (parquet_stats.__isset.min) {
		stats->min = FUNC((const_data_ptr_t)parquet_stats.min.data());
	} else if (parquet_stats.__isset.min_value) {
		stats->min = FUNC((const_data_ptr_t)parquet_stats.min_value.data());
	} else {
		stats->min.is_null = true;
	}
	if (parquet_stats.__isset.max) {
		stats->max = FUNC((const_data_ptr_t)parquet_stats.max.data());
	} else if (parquet_stats.__isset.max_value) {
		stats->max = FUNC((const_data_ptr_t)parquet_stats.max_value.data());
	} else {
		stats->max.is_null = true;
	}
	// GCC 4.x insists on a move() here
	return move(stats);
}

template <class T>
static Value TransformStatisticsPlain(const_data_ptr_t input) {
	return Value::CreateValue<T>(Load<T>(input));
}

static Value TransformStatisticsFloat(const_data_ptr_t input) {
	auto val = Load<float>(input);
	if (!Value::FloatIsValid(val)) {
		return Value(LogicalType::FLOAT);
	}
	return Value::CreateValue<float>(val);
}

static Value TransformStatisticsDouble(const_data_ptr_t input) {
	auto val = Load<double>(input);
	if (!Value::DoubleIsValid(val)) {
		return Value(LogicalType::DOUBLE);
	}
	return Value::CreateValue<double>(val);
}

static Value TransformStatisticsTimestampMs(const_data_ptr_t input) {
	return Value::TIMESTAMP(ParquetTimestampMsToTimestamp(Load<int64_t>(input)));
}

static Value TransformStatisticsTimestampMicros(const_data_ptr_t input) {
	return Value::TIMESTAMP(ParquetTimestampMicrosToTimestamp(Load<int64_t>(input)));
}

static Value TransformStatisticsTimestampImpala(const_data_ptr_t input) {
	return Value::TIMESTAMP(ImpalaTimestampToTimestamp(Load<Int96>(input)));
}

unique_ptr<BaseStatistics> ParquetTransformColumnStatistics(const SchemaElement &s_ele, const LogicalType &type,
                                                            const ColumnChunk &column_chunk) {
	if (!column_chunk.__isset.meta_data || !column_chunk.meta_data.__isset.statistics) {
		// no stats present for row group
		return nullptr;
	}
	auto &parquet_stats = column_chunk.meta_data.statistics;
	unique_ptr<BaseStatistics> row_group_stats;

	switch (type.id()) {

	case LogicalTypeId::UTINYINT:
		row_group_stats = TemplatedGetNumericStats<TransformStatisticsPlain<uint8_t>>(type, parquet_stats);
		break;

	case LogicalTypeId::USMALLINT:
		row_group_stats = TemplatedGetNumericStats<TransformStatisticsPlain<uint16_t>>(type, parquet_stats);
		break;

	case LogicalTypeId::UINTEGER:
		row_group_stats = TemplatedGetNumericStats<TransformStatisticsPlain<uint32_t>>(type, parquet_stats);
		break;

	case LogicalTypeId::UBIGINT:
		row_group_stats = TemplatedGetNumericStats<TransformStatisticsPlain<uint64_t>>(type, parquet_stats);
		break;
	case LogicalTypeId::INTEGER:
		row_group_stats = TemplatedGetNumericStats<TransformStatisticsPlain<int32_t>>(type, parquet_stats);
		break;

	case LogicalTypeId::BIGINT:
		row_group_stats = TemplatedGetNumericStats<TransformStatisticsPlain<int64_t>>(type, parquet_stats);
		break;

	case LogicalTypeId::FLOAT:
		row_group_stats = TemplatedGetNumericStats<TransformStatisticsFloat>(type, parquet_stats);
		break;

	case LogicalTypeId::DOUBLE:
		row_group_stats = TemplatedGetNumericStats<TransformStatisticsDouble>(type, parquet_stats);
		break;

		// here we go, our favorite type
	case LogicalTypeId::TIMESTAMP: {
		switch (s_ele.type) {
		case Type::INT64:
			// arrow timestamp
			switch (s_ele.converted_type) {
			case ConvertedType::TIMESTAMP_MICROS:
				row_group_stats = TemplatedGetNumericStats<TransformStatisticsTimestampMicros>(type, parquet_stats);
				break;
			case ConvertedType::TIMESTAMP_MILLIS:
				row_group_stats = TemplatedGetNumericStats<TransformStatisticsTimestampMs>(type, parquet_stats);
				break;
			default:
				return nullptr;
			}
			break;
		case Type::INT96:
			// impala timestamp
			row_group_stats = TemplatedGetNumericStats<TransformStatisticsTimestampImpala>(type, parquet_stats);
			break;
		default:
			return nullptr;
		}
		break;
	}
	case LogicalTypeId::VARCHAR: {
		auto string_stats = make_unique<StringStatistics>(type);
		if (parquet_stats.__isset.min) {
			string_stats->Update(parquet_stats.min);
		} else if (parquet_stats.__isset.min_value) {
			string_stats->Update(parquet_stats.min_value);
		} else {
			return nullptr;
		}
		if (parquet_stats.__isset.max) {
			string_stats->Update(parquet_stats.max);
		} else if (parquet_stats.__isset.max_value) {
			string_stats->Update(parquet_stats.max_value);
		} else {
			return nullptr;
		}

		string_stats->has_unicode = true; // we dont know better
		row_group_stats = move(string_stats);
		break;
	}
	default:
		// no stats for you
		break;
	} // end of type switch

	// null count is generic
	if (row_group_stats) {
		if (parquet_stats.__isset.null_count) {
			row_group_stats->validity_stats = make_unique<ValidityStatistics>(parquet_stats.null_count != 0);
		} else {
			row_group_stats->validity_stats = make_unique<ValidityStatistics>(true);
		}
	} else {
		// if stats are missing from any row group we know squat
		return nullptr;
	}

	return row_group_stats;
}

} // namespace guinsoodb

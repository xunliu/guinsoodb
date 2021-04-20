//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// parquet_reader.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb.hpp"
#ifndef DUCKDB_AMALGAMATION
#include "guinsoodb/common/common.hpp"
#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/common/string_util.hpp"
#include "guinsoodb/common/types/data_chunk.hpp"
#endif
#include "resizable_buffer.hpp"
#include "column_reader.hpp"

#include "parquet_file_metadata_cache.hpp"
#include "parquet_types.h"
#include "parquet_rle_bp_decoder.hpp"

#include <exception>

namespace parquet {
namespace format {
class FileMetaData;
}
} // namespace parquet

namespace guinsoodb {
class ClientContext;
class ChunkCollection;
class BaseStatistics;
struct TableFilterSet;

struct ParquetReaderScanState {
	vector<idx_t> group_idx_list;
	int64_t current_group;
	vector<column_t> column_ids;
	idx_t group_offset;
	unique_ptr<FileHandle> file_handle;
	unique_ptr<ColumnReader> root_reader;
	unique_ptr<apache::thrift::protocol::TProtocol> thrift_file_proto;

	bool finished;
	TableFilterSet *filters;
	SelectionVector sel;

	ResizeableBuffer define_buf;
	ResizeableBuffer repeat_buf;
};

class ParquetReader {
public:
	ParquetReader(unique_ptr<FileHandle> file_handle_p, const vector<LogicalType> &expected_types_p,
	              const string &initial_filename_p = string());
	ParquetReader(unique_ptr<FileHandle> file_handle_p)
	    : ParquetReader(move(file_handle_p), vector<LogicalType>(), string()) {
	}

	ParquetReader(ClientContext &context, string file_name, const vector<LogicalType> &expected_types_p,
	              const string &initial_filename = string());
	ParquetReader(ClientContext &context, string file_name)
	    : ParquetReader(context, move(file_name), vector<LogicalType>()) {
	}
	~ParquetReader();

	string file_name;
	vector<LogicalType> return_types;
	vector<string> names;
	shared_ptr<ParquetFileMetadataCache> metadata;

public:
	void InitializeScan(ParquetReaderScanState &state, vector<column_t> column_ids, vector<idx_t> groups_to_read,
	                    TableFilterSet *table_filters);
	void Scan(ParquetReaderScanState &state, DataChunk &output);

	idx_t NumRows();
	idx_t NumRowGroups();

	const parquet::format::FileMetaData *GetFileMetadata();

	static unique_ptr<BaseStatistics> ReadStatistics(LogicalType &type, column_t column_index,
	                                                 const parquet::format::FileMetaData *file_meta_data);

private:
	void InitializeSchema(const vector<LogicalType> &expected_types_p, const string &initial_filename_p);
	bool ScanInternal(ParquetReaderScanState &state, DataChunk &output);

	const parquet::format::RowGroup &GetGroup(ParquetReaderScanState &state);
	void PrepareRowGroupBuffer(ParquetReaderScanState &state, idx_t out_col_idx);

	template <typename... Args>
	std::runtime_error FormatException(const string fmt_str, Args... params) {
		return std::runtime_error("Failed to read Parquet file \"" + file_name +
		                          "\": " + StringUtil::Format(fmt_str, params...));
	}

private:
	unique_ptr<FileHandle> file_handle;
};

} // namespace guinsoodb

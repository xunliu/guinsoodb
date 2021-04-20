//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// parquet_writer.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb.hpp"
#ifndef GUINSOODB_AMALGAMATION
#include "guinsoodb/common/common.hpp"
#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/common/mutex.hpp"
#include "guinsoodb/common/serializer/buffered_file_writer.hpp"
#include "guinsoodb/common/types/chunk_collection.hpp"
#endif

#include "parquet_types.h"
#include "thrift/protocol/TCompactProtocol.h"

namespace guinsoodb {
class FileSystem;

class ParquetWriter {
public:
	ParquetWriter(FileSystem &fs, string file_name, vector<LogicalType> types, vector<string> names,
	              parquet::format::CompressionCodec::type codec);

public:
	void Flush(ChunkCollection &buffer);
	void Finalize();

private:
	string file_name;
	vector<LogicalType> sql_types;
	vector<string> column_names;
	parquet::format::CompressionCodec::type codec;

	unique_ptr<BufferedFileWriter> writer;
	shared_ptr<apache::thrift::protocol::TProtocol> protocol;
	parquet::format::FileMetaData file_meta_data;
	std::mutex lock;
};

} // namespace guinsoodb

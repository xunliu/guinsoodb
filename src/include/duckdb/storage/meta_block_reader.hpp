//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/storage/meta_block_reader.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/common.hpp"
#include "guinsoodb/common/serializer.hpp"
#include "guinsoodb/storage/block.hpp"

namespace guinsoodb {
class BlockHandle;
class BufferHandle;
class DatabaseInstance;

//! This struct is responsible for reading meta data from disk
class MetaBlockReader : public Deserializer {
public:
	MetaBlockReader(DatabaseInstance &db, block_id_t block);
	~MetaBlockReader() override;

	DatabaseInstance &db;
	shared_ptr<BlockHandle> block;
	unique_ptr<BufferHandle> handle;
	idx_t offset;
	block_id_t next_block;

public:
	//! Read content of size read_size into the buffer
	void ReadData(data_ptr_t buffer, idx_t read_size) override;

private:
	void ReadNewBlock(block_id_t id);
};
} // namespace guinsoodb

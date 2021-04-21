//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/storage/meta_block_writer.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/common.hpp"
#include "guinsoodb/common/serializer.hpp"
#include "guinsoodb/storage/block.hpp"
#include "guinsoodb/storage/block_manager.hpp"

namespace guinsoodb {
class DatabaseInstance;

//! This struct is responsible for writing metadata to disk
class MetaBlockWriter : public Serializer {
public:
	explicit MetaBlockWriter(DatabaseInstance &db);
	~MetaBlockWriter() override;

	DatabaseInstance &db;
	unique_ptr<Block> block;
	vector<block_id_t> written_blocks;
	idx_t offset;

public:
	void Flush();

	void WriteData(const_data_ptr_t buffer, idx_t write_size) override;
};

} // namespace guinsoodb

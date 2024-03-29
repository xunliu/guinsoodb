//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/storage/checkpoint/write_overflow_strings_to_disk.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/storage/string_segment.hpp"

namespace guinsoodb {

class WriteOverflowStringsToDisk : public OverflowStringWriter {
public:
	explicit WriteOverflowStringsToDisk(DatabaseInstance &db);
	~WriteOverflowStringsToDisk() override;

	//! The checkpoint manager
	DatabaseInstance &db;

	//! Temporary buffer
	unique_ptr<BufferHandle> handle;
	//! The block on-disk to which we are writing
	block_id_t block_id;
	//! The offset within the current block
	idx_t offset;

	static constexpr idx_t STRING_SPACE = Storage::BLOCK_SIZE - sizeof(block_id_t);

public:
	void WriteString(string_t string, block_id_t &result_block, int32_t &result_offset) override;

private:
	void AllocateNewBlock(block_id_t new_block_id);
};

} // namespace guinsoodb

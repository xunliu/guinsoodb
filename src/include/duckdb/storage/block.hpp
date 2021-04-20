//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/storage/block.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/common.hpp"
#include "guinsoodb/storage/storage_info.hpp"
#include "guinsoodb/common/file_buffer.hpp"

namespace guinsoodb {

class Block : public FileBuffer {
public:
	explicit Block(block_id_t id);

	block_id_t id;
};

} // namespace guinsoodb

//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/storage/buffer/buffer_handle.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/storage/storage_info.hpp"

namespace guinsoodb {
class BlockHandle;
class FileBuffer;

class BufferHandle {
public:
	BufferHandle(shared_ptr<BlockHandle> handle, FileBuffer *node);
	~BufferHandle();

	//! The block handle
	shared_ptr<BlockHandle> handle;
	//! The managed buffer node
	FileBuffer *node;
	data_ptr_t Ptr();
};

} // namespace guinsoodb

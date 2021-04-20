#include "guinsoodb/storage/buffer/buffer_handle.hpp"
#include "guinsoodb/storage/buffer_manager.hpp"

namespace guinsoodb {

BufferHandle::BufferHandle(shared_ptr<BlockHandle> handle, FileBuffer *node) : handle(move(handle)), node(node) {
}

BufferHandle::~BufferHandle() {
	auto &buffer_manager = BufferManager::GetBufferManager(handle->db);
	buffer_manager.Unpin(handle);
}

data_ptr_t BufferHandle::Ptr() {
	return node->buffer;
}

} // namespace guinsoodb

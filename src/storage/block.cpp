#include "guinsoodb/storage/block.hpp"

namespace guinsoodb {

Block::Block(block_id_t id) : FileBuffer(FileBufferType::BLOCK, Storage::BLOCK_ALLOC_SIZE), id(id) {
}

} // namespace guinsoodb

//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/common/types/sel_cache.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/types/vector_buffer.hpp"
#include "guinsoodb/common/unordered_map.hpp"

namespace guinsoodb {

//! Selection vector cache used for caching vector slices
struct SelCache {
	unordered_map<sel_t *, buffer_ptr<VectorBuffer>> cache;
};

} // namespace guinsoodb

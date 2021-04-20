//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/storage/object_cache.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/common.hpp"
#include "guinsoodb/common/string.hpp"
#include "guinsoodb/common/unordered_map.hpp"
#include "guinsoodb/common/mutex.hpp"
#include "guinsoodb/main/client_context.hpp"
#include "guinsoodb/main/database.hpp"

namespace guinsoodb {
class ClientContext;

//! ObjectCache is the base class for objects caches in GuinsooDB
class ObjectCacheEntry {
public:
	virtual ~ObjectCacheEntry() {
	}
};

class ObjectCache {
public:
	shared_ptr<ObjectCacheEntry> Get(std::string key) {
		lock_guard<mutex> glock(lock);
		auto entry = cache.find(key);
		if (entry == cache.end()) {
			return nullptr;
		}
		return entry->second;
	}

	void Put(std::string key, shared_ptr<ObjectCacheEntry> value) {
		lock_guard<mutex> glock(lock);
		cache[key] = move(value);
	}

	static ObjectCache &GetObjectCache(ClientContext &context);
	static bool ObjectCacheEnabled(ClientContext &context);

private:
	//! Object Cache
	std::unordered_map<std::string, shared_ptr<ObjectCacheEntry>> cache;
	std::mutex lock;
};

} // namespace guinsoodb

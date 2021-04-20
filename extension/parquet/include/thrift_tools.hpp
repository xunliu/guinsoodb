#pragma once

#include "thrift/protocol/TCompactProtocol.h"
#include "thrift/transport/TBufferTransports.h"

#include "guinsoodb.hpp"
#ifndef GUINSOODB_AMALGAMATION
#include "guinsoodb/common/file_system.hpp"
#endif

namespace guinsoodb {

class ThriftFileTransport : public apache::thrift::transport::TVirtualTransport<ThriftFileTransport> {
public:
	ThriftFileTransport(FileHandle &handle_p) : handle(handle_p), location(0) {
	}

	uint32_t read(uint8_t *buf, uint32_t len) {
		handle.Read(buf, len, location);
		location += len;
		return len;
	}

	void SetLocation(idx_t location_p) {
		location = location_p;
	}

	idx_t GetLocation() {
		return location;
	}
	idx_t GetSize() {
		return handle.file_system.GetFileSize(handle);
	}

private:
	guinsoodb::FileHandle &handle;
	guinsoodb::idx_t location;
};

} // namespace guinsoodb

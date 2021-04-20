#pragma once

#include "httpfs.hpp"
#include "guinsoodb/main/database.hpp"

namespace guinsoodb {

class S3FileSystem : public HTTPFileSystem {
public:
	S3FileSystem(DatabaseInstance &instance_p) : database_instance(instance_p) {
	}
	std::unique_ptr<FileHandle> OpenFile(const char *path, uint8_t flags,
	                                     FileLockType lock = FileLockType::NO_LOCK) override;

	unique_ptr<ResponseWrapper> Request(FileHandle &handle, string url, string method, HeaderMap header_map = {},
	                                    idx_t file_offset = 0, char *buffer_out = nullptr,
	                                    idx_t buffer_len = 0) override;
	static void Verify();

public:
	DatabaseInstance &database_instance;

private:
	HeaderMap CreateAuthHeaders(string host, string path, string method);
};

} // namespace guinsoodb

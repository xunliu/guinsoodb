#include "guinsoodb/common/serializer/buffered_file_writer.hpp"
#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/common/algorithm.hpp"
#include <cstring>

namespace guinsoodb {

BufferedFileWriter::BufferedFileWriter(FileSystem &fs, string path, uint8_t open_flags)
    : fs(fs), data(unique_ptr<data_t[]>(new data_t[FILE_BUFFER_SIZE])), offset(0), total_written(0) {
	handle = fs.OpenFile(path, open_flags, FileLockType::WRITE_LOCK);
}

int64_t BufferedFileWriter::GetFileSize() {
	return fs.GetFileSize(*handle);
}

idx_t BufferedFileWriter::GetTotalWritten() {
	return total_written + offset;
}

void BufferedFileWriter::WriteData(const_data_ptr_t buffer, uint64_t write_size) {
	// first copy anything we can from the buffer
	const_data_ptr_t end_ptr = buffer + write_size;
	while (buffer < end_ptr) {
		idx_t to_write = MinValue<idx_t>((end_ptr - buffer), FILE_BUFFER_SIZE - offset);
		D_ASSERT(to_write > 0);
		memcpy(data.get() + offset, buffer, to_write);
		offset += to_write;
		buffer += to_write;
		if (offset == FILE_BUFFER_SIZE) {
			Flush();
		}
	}
}

void BufferedFileWriter::Flush() {
	if (offset == 0) {
		return;
	}
	fs.Write(*handle, data.get(), offset);
	total_written += offset;
	offset = 0;
}

void BufferedFileWriter::Sync() {
	Flush();
	handle->Sync();
}

void BufferedFileWriter::Truncate(int64_t size) {
	// truncate the physical file on disk
	handle->Truncate(size);
	// reset anything written in the buffer
	offset = 0;
}

} // namespace guinsoodb

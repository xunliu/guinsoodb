//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/common/fstream_util.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/constants.hpp"
#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/common/fstream.hpp"

namespace guinsoodb {
/**
 * Fstream Utility Functions
 */
class FstreamUtil {
public:
	/**
	 * Opens a file for the given name and returns it (default mode : ios_base::in | ios_base::out)
	 */
	static void OpenFile(const string &, fstream &,
	                     ios_base::openmode mode = ios_base::in | ios_base::out | ios::binary);

	/**
	 * Closes the given file or throws an exception otherwise
	 */
	static void CloseFile(fstream &);

	/**
	 * Returns the size in bytes of the given file
	 */
	static idx_t GetFileSize(fstream &);

	/**
	 * Reads the given file as a binary
	 */
	static data_ptr ReadBinary(fstream &);
};
} // namespace guinsoodb

//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/common/types/data_chunk.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/common.hpp"
#include "guinsoodb/common/types/vector.hpp"
#include "guinsoodb/common/winapi.hpp"

struct ArrowArray;

namespace guinsoodb {

//!  A Data Chunk represents a set of vectors.
/*!
    The data chunk class is the intermediate representation used by the
   execution engine of GuinsooDB. It effectively represents a subset of a relation.
   It holds a set of vectors that all have the same length.

    DataChunk is initialized using the DataChunk::Initialize function by
   providing it with a vector of TypeIds for the Vector members. By default,
   this function will also allocate a chunk of memory in the DataChunk for the
   vectors and all the vectors will be referencing vectors to the data owned by
   the chunk. The reason for this behavior is that the underlying vectors can
   become referencing vectors to other chunks as well (i.e. in the case an
   operator does not alter the data, such as a Filter operator which only adds a
   selection vector).

    In addition to holding the data of the vectors, the DataChunk also owns the
   selection vector that underlying vectors can point to.
*/
class DataChunk {
public:
	//! Creates an empty DataChunk
	DataChunk();

	//! The vectors owned by the DataChunk.
	vector<Vector> data;

public:
	GUINSOODB_API idx_t size() const {
		return count;
	}
	GUINSOODB_API idx_t ColumnCount() const {
		return data.size();
	}
	void SetCardinality(idx_t count) {
		D_ASSERT(count <= STANDARD_VECTOR_SIZE);
		this->count = count;
	}
	void SetCardinality(const DataChunk &other) {
		this->count = other.size();
	}

	GUINSOODB_API Value GetValue(idx_t col_idx, idx_t index) const;
	GUINSOODB_API void SetValue(idx_t col_idx, idx_t index, const Value &val);

	//! Set the DataChunk to reference another data chunk
	GUINSOODB_API void Reference(DataChunk &chunk);

	//! Initializes the DataChunk with the specified types to an empty DataChunk
	//! This will create one vector of the specified type for each LogicalType in the
	//! types list. The vector will be referencing vector to the data owned by
	//! the DataChunk.
	void Initialize(const vector<LogicalType> &types);
	//! Initializes an empty DataChunk with the given types. The vectors will *not* have any data allocated for them.
	void InitializeEmpty(const vector<LogicalType> &types);
	//! Append the other DataChunk to this one. The column count and types of
	//! the two DataChunks have to match exactly. Throws an exception if there
	//! is not enough space in the chunk.
	GUINSOODB_API void Append(const DataChunk &other);
	//! Destroy all data and columns owned by this DataChunk
	GUINSOODB_API void Destroy();

	//! Copies the data from this vector to another vector.
	GUINSOODB_API void Copy(DataChunk &other, idx_t offset = 0) const;
	GUINSOODB_API void Copy(DataChunk &other, const SelectionVector &sel, const idx_t source_count,
	                     const idx_t offset = 0) const;

	//! Turn all the vectors from the chunk into flat vectors
	GUINSOODB_API void Normalify();

	GUINSOODB_API unique_ptr<VectorData[]> Orrify();

	GUINSOODB_API void Slice(const SelectionVector &sel_vector, idx_t count);
	GUINSOODB_API void Slice(DataChunk &other, const SelectionVector &sel, idx_t count, idx_t col_offset = 0);

	//! Resets the DataChunk to its state right after the DataChunk::Initialize
	//! function was called. This sets the count to 0, and resets each member
	//! Vector to point back to the data owned by this DataChunk.
	GUINSOODB_API void Reset();

	//! Serializes a DataChunk to a stand-alone binary blob
	GUINSOODB_API void Serialize(Serializer &serializer);
	//! Deserializes a blob back into a DataChunk
	GUINSOODB_API void Deserialize(Deserializer &source);

	//! Hashes the DataChunk to the target vector
	GUINSOODB_API void Hash(Vector &result);

	//! Returns a list of types of the vectors of this data chunk
	GUINSOODB_API vector<LogicalType> GetTypes();

	//! Converts this DataChunk to a printable string representation
	GUINSOODB_API string ToString() const;
	GUINSOODB_API void Print();

	DataChunk(const DataChunk &) = delete;

	//! Verify that the DataChunk is in a consistent, not corrupt state. DEBUG
	//! FUNCTION ONLY!
	GUINSOODB_API void Verify();

	//! export data chunk as a arrow struct array that can be imported as arrow record batch
	GUINSOODB_API void ToArrowArray(ArrowArray *out_array);

private:
	idx_t count;
};
} // namespace guinsoodb

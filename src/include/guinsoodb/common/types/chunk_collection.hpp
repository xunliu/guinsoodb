//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/common/types/chunk_collection.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/enums/order_type.hpp"
#include "guinsoodb/common/types/data_chunk.hpp"
#include "guinsoodb/common/winapi.hpp"

namespace guinsoodb {

//!  A ChunkCollection represents a set of DataChunks that all have the same
//!  types
/*!
    A ChunkCollection represents a set of DataChunks concatenated together in a
   list. Individual values of the collection can be iterated over using the
   iterator. It is also possible to iterate directly over the chunks for more
   direct access.
*/
class ChunkCollection {
public:
	ChunkCollection() : count(0) {
	}

	//! The amount of columns in the ChunkCollection
	GUINSOODB_API vector<LogicalType> &Types() {
		return types;
	}
	const vector<LogicalType> &Types() const {
		return types;
	}

	//! The amount of rows in the ChunkCollection
	GUINSOODB_API const idx_t &Count() const {
		return count;
	}

	//! The amount of columns in the ChunkCollection
	GUINSOODB_API idx_t ColumnCount() const {
		return types.size();
	}

	//! Append a new DataChunk directly to this ChunkCollection
	GUINSOODB_API void Append(DataChunk &new_chunk);

	//! Append another ChunkCollection directly to this ChunkCollection
	GUINSOODB_API void Append(ChunkCollection &other);

	//! Merge is like Append but messes up the order and destroys the other collection
	GUINSOODB_API void Merge(ChunkCollection &other);

	GUINSOODB_API void Verify();

	//! Gets the value of the column at the specified index
	GUINSOODB_API Value GetValue(idx_t column, idx_t index);
	//! Sets the value of the column at the specified index
	GUINSOODB_API void SetValue(idx_t column, idx_t index, const Value &value);

	GUINSOODB_API vector<Value> GetRow(idx_t index);

	GUINSOODB_API string ToString() const {
		return chunks.size() == 0 ? "ChunkCollection [ 0 ]"
		                          : "ChunkCollection [ " + std::to_string(count) + " ]: \n" + chunks[0]->ToString();
	}
	GUINSOODB_API void Print();

	//! Gets a reference to the chunk at the given index
	GUINSOODB_API DataChunk &GetChunkForRow(idx_t row_index) {
		return *chunks[LocateChunk(row_index)];
	}

	//! Gets a reference to the chunk at the given index
	GUINSOODB_API DataChunk &GetChunk(idx_t chunk_index) {
		D_ASSERT(chunk_index < chunks.size());
		return *chunks[chunk_index];
	}
	const DataChunk &GetChunk(idx_t chunk_index) const {
		D_ASSERT(chunk_index < chunks.size());
		return *chunks[chunk_index];
	}

	GUINSOODB_API const vector<unique_ptr<DataChunk>> &Chunks() {
		return chunks;
	}

	GUINSOODB_API idx_t ChunkCount() const {
		return chunks.size();
	}

	GUINSOODB_API void Reset() {
		count = 0;
		chunks.clear();
		types.clear();
	}

	GUINSOODB_API unique_ptr<DataChunk> Fetch() {
		if (ChunkCount() == 0) {
			return nullptr;
		}

		auto res = move(chunks[0]);
		chunks.erase(chunks.begin() + 0);
		return res;
	}

	GUINSOODB_API void Sort(vector<OrderType> &desc, vector<OrderByNullType> &null_order, idx_t result[]);
	//! Reorders the rows in the collection according to the given indices.
	GUINSOODB_API void Reorder(idx_t order[]);

	GUINSOODB_API void MaterializeSortedChunk(DataChunk &target, idx_t order[], idx_t start_offset);

	//! Returns true if the ChunkCollections are equivalent
	GUINSOODB_API bool Equals(ChunkCollection &other);

	//! Locates the chunk that belongs to the specific index
	GUINSOODB_API idx_t LocateChunk(idx_t index) {
		idx_t result = index / STANDARD_VECTOR_SIZE;
		D_ASSERT(result < chunks.size());
		return result;
	}

	GUINSOODB_API void Heap(vector<OrderType> &desc, vector<OrderByNullType> &null_order, idx_t heap[], idx_t heap_size);
	GUINSOODB_API idx_t MaterializeHeapChunk(DataChunk &target, idx_t order[], idx_t start_offset, idx_t heap_size);

private:
	//! The total amount of elements in the collection
	idx_t count;
	//! The set of data chunks in the collection
	vector<unique_ptr<DataChunk>> chunks;
	//! The types of the ChunkCollection
	vector<LogicalType> types;
};
} // namespace guinsoodb

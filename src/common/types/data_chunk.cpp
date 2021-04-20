#include "guinsoodb/common/types/data_chunk.hpp"

#include "guinsoodb/common/array.hpp"
#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/common/helper.hpp"
#include "guinsoodb/common/printer.hpp"
#include "guinsoodb/common/serializer.hpp"
#include "guinsoodb/common/types/null_value.hpp"
#include "guinsoodb/common/types/date.hpp"
#include "guinsoodb/common/types/timestamp.hpp"
#include "guinsoodb/common/vector_operations/vector_operations.hpp"
#include "guinsoodb/common/unordered_map.hpp"
#include "guinsoodb/common/types/sel_cache.hpp"
#include "guinsoodb/common/arrow.hpp"
#include "guinsoodb/common/vector.hpp"
#include "guinsoodb/common/to_string.hpp"

namespace guinsoodb {

DataChunk::DataChunk() : count(0) {
}

void DataChunk::InitializeEmpty(const vector<LogicalType> &types) {
	D_ASSERT(types.size() > 0);
	for (idx_t i = 0; i < types.size(); i++) {
		data.emplace_back(Vector(types[i], nullptr));
	}
}

void DataChunk::Initialize(const vector<LogicalType> &types) {
	D_ASSERT(types.size() > 0);
	InitializeEmpty(types);
	for (idx_t i = 0; i < types.size(); i++) {
		data[i].Initialize();
	}
}

void DataChunk::Reset() {
	for (idx_t i = 0; i < ColumnCount(); i++) {
		data[i].Initialize();
	}
	SetCardinality(0);
}

void DataChunk::Destroy() {
	data.clear();
	SetCardinality(0);
}

Value DataChunk::GetValue(idx_t col_idx, idx_t index) const {
	D_ASSERT(index < size());
	return data[col_idx].GetValue(index);
}

void DataChunk::SetValue(idx_t col_idx, idx_t index, const Value &val) {
	data[col_idx].SetValue(index, val);
}

void DataChunk::Reference(DataChunk &chunk) {
	D_ASSERT(chunk.ColumnCount() <= ColumnCount());
	SetCardinality(chunk);
	for (idx_t i = 0; i < chunk.ColumnCount(); i++) {
		data[i].Reference(chunk.data[i]);
	}
}

void DataChunk::Copy(DataChunk &other, idx_t offset) const {
	D_ASSERT(ColumnCount() == other.ColumnCount());
	D_ASSERT(other.size() == 0);

	for (idx_t i = 0; i < ColumnCount(); i++) {
		D_ASSERT(other.data[i].GetVectorType() == VectorType::FLAT_VECTOR);
		VectorOperations::Copy(data[i], other.data[i], size(), offset, 0);
	}
	other.SetCardinality(size() - offset);
}

void DataChunk::Copy(DataChunk &other, const SelectionVector &sel, const idx_t source_count, const idx_t offset) const {
	D_ASSERT(ColumnCount() == other.ColumnCount());
	D_ASSERT(other.size() == 0);
	D_ASSERT((offset + source_count) <= size());

	for (idx_t i = 0; i < ColumnCount(); i++) {
		D_ASSERT(other.data[i].GetVectorType() == VectorType::FLAT_VECTOR);
		VectorOperations::Copy(data[i], other.data[i], sel, source_count, offset, 0);
	}
	other.SetCardinality(source_count - offset);
}

void DataChunk::Append(const DataChunk &other) {
	if (other.size() == 0) {
		return;
	}
	if (ColumnCount() != other.ColumnCount()) {
		throw OutOfRangeException("Column counts of appending chunk doesn't match!");
	}
	for (idx_t i = 0; i < ColumnCount(); i++) {
		D_ASSERT(data[i].GetVectorType() == VectorType::FLAT_VECTOR);
		VectorOperations::Copy(other.data[i], data[i], other.size(), 0, size());
	}
	SetCardinality(size() + other.size());
}

void DataChunk::Normalify() {
	for (idx_t i = 0; i < ColumnCount(); i++) {
		data[i].Normalify(size());
	}
}

vector<LogicalType> DataChunk::GetTypes() {
	vector<LogicalType> types;
	for (idx_t i = 0; i < ColumnCount(); i++) {
		types.push_back(data[i].GetType());
	}
	return types;
}

string DataChunk::ToString() const {
	string retval = "Chunk - [" + to_string(ColumnCount()) + " Columns]\n";
	for (idx_t i = 0; i < ColumnCount(); i++) {
		retval += "- " + data[i].ToString(size()) + "\n";
	}
	return retval;
}

void DataChunk::Serialize(Serializer &serializer) {
	// write the count
	serializer.Write<sel_t>(size());
	serializer.Write<idx_t>(ColumnCount());
	for (idx_t col_idx = 0; col_idx < ColumnCount(); col_idx++) {
		// write the types
		data[col_idx].GetType().Serialize(serializer);
	}
	// write the data
	for (idx_t col_idx = 0; col_idx < ColumnCount(); col_idx++) {
		data[col_idx].Serialize(size(), serializer);
	}
}

void DataChunk::Deserialize(Deserializer &source) {
	auto rows = source.Read<sel_t>();
	idx_t column_count = source.Read<idx_t>();

	vector<LogicalType> types;
	for (idx_t i = 0; i < column_count; i++) {
		types.push_back(LogicalType::Deserialize(source));
	}
	Initialize(types);
	// now load the column data
	SetCardinality(rows);
	for (idx_t i = 0; i < column_count; i++) {
		data[i].Deserialize(rows, source);
	}
	Verify();
}

void DataChunk::Slice(const SelectionVector &sel_vector, idx_t count) {
	this->count = count;
	SelCache merge_cache;
	for (idx_t c = 0; c < ColumnCount(); c++) {
		data[c].Slice(sel_vector, count, merge_cache);
	}
}

void DataChunk::Slice(DataChunk &other, const SelectionVector &sel, idx_t count, idx_t col_offset) {
	D_ASSERT(other.ColumnCount() <= col_offset + ColumnCount());
	this->count = count;
	SelCache merge_cache;
	for (idx_t c = 0; c < other.ColumnCount(); c++) {
		if (other.data[c].GetVectorType() == VectorType::DICTIONARY_VECTOR) {
			// already a dictionary! merge the dictionaries
			data[col_offset + c].Reference(other.data[c]);
			data[col_offset + c].Slice(sel, count, merge_cache);
		} else {
			data[col_offset + c].Slice(other.data[c], sel, count);
		}
	}
}

unique_ptr<VectorData[]> DataChunk::Orrify() {
	auto orrified_data = unique_ptr<VectorData[]>(new VectorData[ColumnCount()]);
	for (idx_t col_idx = 0; col_idx < ColumnCount(); col_idx++) {
		data[col_idx].Orrify(size(), orrified_data[col_idx]);
	}
	return orrified_data;
}

void DataChunk::Hash(Vector &result) {
	D_ASSERT(result.GetType().id() == LogicalTypeId::HASH);
	VectorOperations::Hash(data[0], result, size());
	for (idx_t i = 1; i < ColumnCount(); i++) {
		VectorOperations::CombineHash(result, data[i], size());
	}
}

void DataChunk::Verify() {
#ifdef DEBUG
	D_ASSERT(size() <= STANDARD_VECTOR_SIZE);
	// verify that all vectors in this chunk have the chunk selection vector
	for (idx_t i = 0; i < ColumnCount(); i++) {
		data[i].Verify(size());
	}
#endif
}

void DataChunk::Print() {
	Printer::Print(ToString());
}

struct GuinsooDBArrowArrayChildHolder {
	ArrowArray array;
	// need max three pointers for strings
	guinsoodb::array<const void *, 3> buffers = {{nullptr, nullptr, nullptr}};
	Vector vector = {};
	unique_ptr<data_t[]> string_offsets = nullptr;
	unique_ptr<data_t[]> string_data = nullptr;
};

struct GuinsooDBArrowArrayHolder {
	vector<GuinsooDBArrowArrayChildHolder> children = {};
	vector<ArrowArray *> children_ptrs = {};
	array<const void *, 1> buffers = {{nullptr}};
};

static void ReleaseGuinsooDBArrowArray(ArrowArray *array) {
	if (!array || !array->release) {
		return;
	}
	array->release = nullptr;
	auto holder = static_cast<GuinsooDBArrowArrayHolder *>(array->private_data);
	delete holder;
}

void DataChunk::ToArrowArray(ArrowArray *out_array) {
	Normalify();
	D_ASSERT(out_array);

	// Allocate as unique_ptr first to cleanup properly on error
	auto root_holder = make_unique<GuinsooDBArrowArrayHolder>();

	// Allocate the children
	root_holder->children.resize(ColumnCount());
	root_holder->children_ptrs.resize(ColumnCount(), nullptr);
	for (size_t i = 0; i < ColumnCount(); ++i) {
		root_holder->children_ptrs[i] = &root_holder->children[i].array;
	}
	out_array->children = root_holder->children_ptrs.data();
	out_array->n_children = ColumnCount();

	// Configure root array
	out_array->length = size();
	out_array->n_children = ColumnCount();
	out_array->n_buffers = 1;
	out_array->buffers = root_holder->buffers.data(); // there is no actual buffer there since we don't have NULLs
	out_array->offset = 0;
	out_array->null_count = 0; // needs to be 0
	out_array->dictionary = nullptr;

	// Configure child arrays
	for (idx_t col_idx = 0; col_idx < ColumnCount(); col_idx++) {
		auto &child_holder = root_holder->children[col_idx];
		auto &child = child_holder.array;

		auto &vector = child_holder.vector;
		vector.Reference(data[col_idx]);

		child.private_data = nullptr;
		child.release = ReleaseGuinsooDBArrowArray;
		child.n_children = 0;
		child.null_count = -1; // unknown
		child.offset = 0;
		child.dictionary = nullptr;
		child.buffers = child_holder.buffers.data();

		child.length = size();

		switch (vector.GetVectorType()) {
			// TODO support other vector types
		case VectorType::FLAT_VECTOR: {
			switch (GetTypes()[col_idx].id()) {
				// TODO support other data types
			case LogicalTypeId::BOOLEAN:
			case LogicalTypeId::TINYINT:
			case LogicalTypeId::SMALLINT:
			case LogicalTypeId::INTEGER:
			case LogicalTypeId::BIGINT:
			case LogicalTypeId::UTINYINT:
			case LogicalTypeId::USMALLINT:
			case LogicalTypeId::UINTEGER:
			case LogicalTypeId::UBIGINT:
			case LogicalTypeId::FLOAT:
			case LogicalTypeId::DOUBLE:
			case LogicalTypeId::HUGEINT:
			case LogicalTypeId::DATE:
				child.n_buffers = 2;
				child.buffers[1] = (void *)FlatVector::GetData(vector);
				break;
			case LogicalTypeId::TIME: {
				// convert time from microseconds to miliseconds
				child.n_buffers = 2;
				child_holder.string_data = unique_ptr<data_t[]>(new data_t[sizeof(uint32_t) * (size() + 1)]);
				child.buffers[1] = child_holder.string_data.get();
				auto source_ptr = FlatVector::GetData<dtime_t>(vector);
				auto target_ptr = (uint32_t *)child.buffers[1];
				for (idx_t row_idx = 0; row_idx < size(); row_idx++) {
					target_ptr[row_idx] = uint32_t(source_ptr[row_idx] / 1000);
				}
				break;
			}
			case LogicalTypeId::TIMESTAMP: {
				// convert timestamp from microseconds to nanoseconds
				child.n_buffers = 2;
				child.buffers[1] = (void *)FlatVector::GetData(vector);
				auto target_ptr = (timestamp_t *)child.buffers[1];
				for (idx_t row_idx = 0; row_idx < size(); row_idx++) {
					target_ptr[row_idx] = Timestamp::GetEpochNanoSeconds(target_ptr[row_idx]);
				}
				break;
			}

			case LogicalTypeId::VARCHAR: {
				child.n_buffers = 3;
				child_holder.string_offsets = unique_ptr<data_t[]>(new data_t[sizeof(uint32_t) * (size() + 1)]);
				child.buffers[1] = child_holder.string_offsets.get();
				D_ASSERT(child.buffers[1]);
				// step 1: figure out total string length:
				idx_t total_string_length = 0;
				auto string_t_ptr = FlatVector::GetData<string_t>(vector);
				auto &mask = FlatVector::Validity(vector);
				for (idx_t row_idx = 0; row_idx < size(); row_idx++) {
					if (!mask.RowIsValid(row_idx)) {
						continue;
					}
					total_string_length += string_t_ptr[row_idx].GetSize();
				}
				// step 2: allocate this much
				child_holder.string_data = unique_ptr<data_t[]>(new data_t[total_string_length]);
				child.buffers[2] = child_holder.string_data.get();
				D_ASSERT(child.buffers[2]);
				// step 3: assign buffers
				idx_t current_heap_offset = 0;
				auto target_ptr = (uint32_t *)child.buffers[1];

				for (idx_t row_idx = 0; row_idx < size(); row_idx++) {
					target_ptr[row_idx] = current_heap_offset;
					if (!mask.RowIsValid(row_idx)) {
						continue;
					}
					auto &str = string_t_ptr[row_idx];
					memcpy((void *)((uint8_t *)child.buffers[2] + current_heap_offset), str.GetDataUnsafe(),
					       str.GetSize());
					current_heap_offset += str.GetSize();
				}
				target_ptr[size()] = current_heap_offset; // need to terminate last string!
				break;
			}
			default:
				throw std::runtime_error("Unsupported type " + GetTypes()[col_idx].ToString());
			}

			auto &mask = FlatVector::Validity(vector);
			if (!mask.AllValid()) {
				// any bits are set: might have nulls
				child.null_count = -1;
			} else {
				// no bits are set; we know there are no nulls
				child.null_count = 0;
			}
			child.buffers[0] = (void *)mask.GetData();
			break;
		}
		default:
			throw NotImplementedException(VectorTypeToString(vector.GetVectorType()));
		}
		out_array->children[col_idx] = &child;
	}

	// Release ownership to caller
	out_array->private_data = root_holder.release();
	out_array->release = ReleaseGuinsooDBArrowArray;
}

} // namespace guinsoodb

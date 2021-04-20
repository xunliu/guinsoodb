//===--------------------------------------------------------------------===//
// generators.cpp
// Description: This file contains the implementation of different generators
//===--------------------------------------------------------------------===//

#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/common/vector_operations/vector_operations.hpp"
#include "guinsoodb/common/limits.hpp"

namespace guinsoodb {

template <class T>
void TemplatedGenerateSequence(Vector &result, idx_t count, int64_t start, int64_t increment) {
	D_ASSERT(result.GetType().IsNumeric());
	if (start > NumericLimits<T>::Maximum() || increment > NumericLimits<T>::Maximum()) {
		throw Exception("Sequence start or increment out of type range");
	}
	result.SetVectorType(VectorType::FLAT_VECTOR);
	auto result_data = FlatVector::GetData<T>(result);
	auto value = (T)start;
	for (idx_t i = 0; i < count; i++) {
		result_data[i] = value;
		value += increment;
	}
}

void VectorOperations::GenerateSequence(Vector &result, idx_t count, int64_t start, int64_t increment) {
	if (!result.GetType().IsNumeric()) {
		throw InvalidTypeException(result.GetType(), "Can only generate sequences for numeric values!");
	}
	switch (result.GetType().InternalType()) {
	case PhysicalType::INT8:
		TemplatedGenerateSequence<int8_t>(result, count, start, increment);
		break;
	case PhysicalType::INT16:
		TemplatedGenerateSequence<int16_t>(result, count, start, increment);
		break;
	case PhysicalType::INT32:
		TemplatedGenerateSequence<int32_t>(result, count, start, increment);
		break;
	case PhysicalType::INT64:
		TemplatedGenerateSequence<int64_t>(result, count, start, increment);
		break;
	case PhysicalType::FLOAT:
		TemplatedGenerateSequence<float>(result, count, start, increment);
		break;
	case PhysicalType::DOUBLE:
		TemplatedGenerateSequence<double>(result, count, start, increment);
		break;
	default:
		throw NotImplementedException("Unimplemented type for generate sequence");
	}
}

template <class T>
void TemplatedGenerateSequence(Vector &result, idx_t count, const SelectionVector &sel, int64_t start,
                               int64_t increment) {
	D_ASSERT(result.GetType().IsNumeric());
	if (start > NumericLimits<T>::Maximum() || increment > NumericLimits<T>::Maximum()) {
		throw Exception("Sequence start or increment out of type range");
	}
	result.SetVectorType(VectorType::FLAT_VECTOR);
	auto result_data = FlatVector::GetData<T>(result);
	auto value = (T)start;
	for (idx_t i = 0; i < count; i++) {
		auto idx = sel.get_index(i);
		result_data[idx] = value + increment * idx;
	}
}

void VectorOperations::GenerateSequence(Vector &result, idx_t count, const SelectionVector &sel, int64_t start,
                                        int64_t increment) {
	if (!result.GetType().IsNumeric()) {
		throw InvalidTypeException(result.GetType(), "Can only generate sequences for numeric values!");
	}
	switch (result.GetType().InternalType()) {
	case PhysicalType::INT8:
		TemplatedGenerateSequence<int8_t>(result, count, sel, start, increment);
		break;
	case PhysicalType::INT16:
		TemplatedGenerateSequence<int16_t>(result, count, sel, start, increment);
		break;
	case PhysicalType::INT32:
		TemplatedGenerateSequence<int32_t>(result, count, sel, start, increment);
		break;
	case PhysicalType::INT64:
		TemplatedGenerateSequence<int64_t>(result, count, sel, start, increment);
		break;
	case PhysicalType::FLOAT:
		TemplatedGenerateSequence<float>(result, count, sel, start, increment);
		break;
	case PhysicalType::DOUBLE:
		TemplatedGenerateSequence<double>(result, count, sel, start, increment);
		break;
	default:
		throw NotImplementedException("Unimplemented type for generate sequence");
	}
}

} // namespace guinsoodb

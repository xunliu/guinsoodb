//===--------------------------------------------------------------------===//
// hash.cpp
// Description: This file contains the vectorized hash implementations
//===--------------------------------------------------------------------===//

#include "guinsoodb/common/vector_operations/vector_operations.hpp"
#include "guinsoodb/common/types/hash.hpp"
#include "guinsoodb/common/types/null_value.hpp"

namespace guinsoodb {

struct HashOp {
	template <class T>
	static inline hash_t Operation(T input, bool is_null) {
		return guinsoodb::Hash<T>(is_null ? guinsoodb::NullValue<T>() : input);
	}
};

template <bool HAS_RSEL, class T>
static inline void TightLoopHash(T *__restrict ldata, hash_t *__restrict result_data, const SelectionVector *rsel,
                                 idx_t count, const SelectionVector *__restrict sel_vector, ValidityMask &mask) {
	if (!mask.AllValid()) {
		for (idx_t i = 0; i < count; i++) {
			auto ridx = HAS_RSEL ? rsel->get_index(i) : i;
			auto idx = sel_vector->get_index(ridx);
			result_data[ridx] = HashOp::Operation(ldata[idx], !mask.RowIsValid(idx));
		}
	} else {
		for (idx_t i = 0; i < count; i++) {
			auto ridx = HAS_RSEL ? rsel->get_index(i) : i;
			auto idx = sel_vector->get_index(ridx);
			result_data[ridx] = guinsoodb::Hash<T>(ldata[idx]);
		}
	}
}

template <bool HAS_RSEL, class T>
static inline void TemplatedLoopHash(Vector &input, Vector &result, const SelectionVector *rsel, idx_t count) {
	if (input.GetVectorType() == VectorType::CONSTANT_VECTOR) {
		result.SetVectorType(VectorType::CONSTANT_VECTOR);

		auto ldata = ConstantVector::GetData<T>(input);
		auto result_data = ConstantVector::GetData<hash_t>(result);
		*result_data = HashOp::Operation(*ldata, ConstantVector::IsNull(input));
	} else {
		result.SetVectorType(VectorType::FLAT_VECTOR);

		VectorData idata;
		input.Orrify(count, idata);

		TightLoopHash<HAS_RSEL, T>((T *)idata.data, FlatVector::GetData<hash_t>(result), rsel, count, idata.sel,
		                           idata.validity);
	}
}

template <bool HAS_RSEL>
static inline void HashTypeSwitch(Vector &input, Vector &result, const SelectionVector *rsel, idx_t count) {
	D_ASSERT(result.GetType().id() == LogicalTypeId::HASH);
	switch (input.GetType().InternalType()) {
	case PhysicalType::BOOL:
	case PhysicalType::INT8:
		TemplatedLoopHash<HAS_RSEL, int8_t>(input, result, rsel, count);
		break;
	case PhysicalType::INT16:
		TemplatedLoopHash<HAS_RSEL, int16_t>(input, result, rsel, count);
		break;
	case PhysicalType::INT32:
		TemplatedLoopHash<HAS_RSEL, int32_t>(input, result, rsel, count);
		break;
	case PhysicalType::INT64:
		TemplatedLoopHash<HAS_RSEL, int64_t>(input, result, rsel, count);
		break;
	case PhysicalType::UINT8:
		TemplatedLoopHash<HAS_RSEL, uint8_t>(input, result, rsel, count);
		break;
	case PhysicalType::UINT16:
		TemplatedLoopHash<HAS_RSEL, uint16_t>(input, result, rsel, count);
		break;
	case PhysicalType::UINT32:
		TemplatedLoopHash<HAS_RSEL, uint32_t>(input, result, rsel, count);
		break;
	case PhysicalType::UINT64:
		TemplatedLoopHash<HAS_RSEL, uint64_t>(input, result, rsel, count);
		break;
	case PhysicalType::INT128:
		TemplatedLoopHash<HAS_RSEL, hugeint_t>(input, result, rsel, count);
		break;
	case PhysicalType::FLOAT:
		TemplatedLoopHash<HAS_RSEL, float>(input, result, rsel, count);
		break;
	case PhysicalType::DOUBLE:
		TemplatedLoopHash<HAS_RSEL, double>(input, result, rsel, count);
		break;
	case PhysicalType::INTERVAL:
		TemplatedLoopHash<HAS_RSEL, interval_t>(input, result, rsel, count);
		break;
	case PhysicalType::VARCHAR:
		TemplatedLoopHash<HAS_RSEL, string_t>(input, result, rsel, count);
		break;
	default:
		throw InvalidTypeException(input.GetType(), "Invalid type for hash");
	}
}

void VectorOperations::Hash(Vector &input, Vector &result, idx_t count) {
	HashTypeSwitch<false>(input, result, nullptr, count);
}

void VectorOperations::Hash(Vector &input, Vector &result, const SelectionVector &sel, idx_t count) {
	HashTypeSwitch<true>(input, result, &sel, count);
}

static inline hash_t CombineHashScalar(hash_t a, hash_t b) {
	return (a * UINT64_C(0xbf58476d1ce4e5b9)) ^ b;
}

template <bool HAS_RSEL, class T>
static inline void TightLoopCombineHashConstant(T *__restrict ldata, hash_t constant_hash, hash_t *__restrict hash_data,
                                                const SelectionVector *rsel, idx_t count,
                                                const SelectionVector *__restrict sel_vector, ValidityMask &mask) {
	if (!mask.AllValid()) {
		for (idx_t i = 0; i < count; i++) {
			auto ridx = HAS_RSEL ? rsel->get_index(i) : i;
			auto idx = sel_vector->get_index(ridx);
			auto other_hash = HashOp::Operation(ldata[idx], !mask.RowIsValid(idx));
			hash_data[ridx] = CombineHashScalar(constant_hash, other_hash);
		}
	} else {
		for (idx_t i = 0; i < count; i++) {
			auto ridx = HAS_RSEL ? rsel->get_index(i) : i;
			auto idx = sel_vector->get_index(ridx);
			auto other_hash = guinsoodb::Hash<T>(ldata[idx]);
			hash_data[ridx] = CombineHashScalar(constant_hash, other_hash);
		}
	}
}

template <bool HAS_RSEL, class T>
static inline void TightLoopCombineHash(T *__restrict ldata, hash_t *__restrict hash_data, const SelectionVector *rsel,
                                        idx_t count, const SelectionVector *__restrict sel_vector, ValidityMask &mask) {
	if (!mask.AllValid()) {
		for (idx_t i = 0; i < count; i++) {
			auto ridx = HAS_RSEL ? rsel->get_index(i) : i;
			auto idx = sel_vector->get_index(ridx);
			auto other_hash = HashOp::Operation(ldata[idx], !mask.RowIsValid(idx));
			hash_data[ridx] = CombineHashScalar(hash_data[ridx], other_hash);
		}
	} else {
		for (idx_t i = 0; i < count; i++) {
			auto ridx = HAS_RSEL ? rsel->get_index(i) : i;
			auto idx = sel_vector->get_index(ridx);
			auto other_hash = guinsoodb::Hash<T>(ldata[idx]);
			hash_data[ridx] = CombineHashScalar(hash_data[ridx], other_hash);
		}
	}
}

template <bool HAS_RSEL, class T>
void TemplatedLoopCombineHash(Vector &input, Vector &hashes, const SelectionVector *rsel, idx_t count) {
	if (input.GetVectorType() == VectorType::CONSTANT_VECTOR && hashes.GetVectorType() == VectorType::CONSTANT_VECTOR) {
		auto ldata = ConstantVector::GetData<T>(input);
		auto hash_data = ConstantVector::GetData<hash_t>(hashes);

		auto other_hash = HashOp::Operation(*ldata, ConstantVector::IsNull(input));
		*hash_data = CombineHashScalar(*hash_data, other_hash);
	} else {
		VectorData idata;
		input.Orrify(count, idata);
		if (hashes.GetVectorType() == VectorType::CONSTANT_VECTOR) {
			// mix constant with non-constant, first get the constant value
			auto constant_hash = *ConstantVector::GetData<hash_t>(hashes);
			// now re-initialize the hashes vector to an empty flat vector
			hashes.Initialize(hashes.GetType());
			TightLoopCombineHashConstant<HAS_RSEL, T>((T *)idata.data, constant_hash,
			                                          FlatVector::GetData<hash_t>(hashes), rsel, count, idata.sel,
			                                          idata.validity);
		} else {
			D_ASSERT(hashes.GetVectorType() == VectorType::FLAT_VECTOR);
			TightLoopCombineHash<HAS_RSEL, T>((T *)idata.data, FlatVector::GetData<hash_t>(hashes), rsel, count,
			                                  idata.sel, idata.validity);
		}
	}
}

template <bool HAS_RSEL>
static inline void CombineHashTypeSwitch(Vector &hashes, Vector &input, const SelectionVector *rsel, idx_t count) {
	D_ASSERT(hashes.GetType().id() == LogicalTypeId::HASH);
	switch (input.GetType().InternalType()) {
	case PhysicalType::BOOL:
	case PhysicalType::INT8:
		TemplatedLoopCombineHash<HAS_RSEL, int8_t>(input, hashes, rsel, count);
		break;
	case PhysicalType::INT16:
		TemplatedLoopCombineHash<HAS_RSEL, int16_t>(input, hashes, rsel, count);
		break;
	case PhysicalType::INT32:
		TemplatedLoopCombineHash<HAS_RSEL, int32_t>(input, hashes, rsel, count);
		break;
	case PhysicalType::INT64:
		TemplatedLoopCombineHash<HAS_RSEL, int64_t>(input, hashes, rsel, count);
		break;
	case PhysicalType::UINT8:
		TemplatedLoopCombineHash<HAS_RSEL, uint8_t>(input, hashes, rsel, count);
		break;
	case PhysicalType::UINT16:
		TemplatedLoopCombineHash<HAS_RSEL, uint16_t>(input, hashes, rsel, count);
		break;
	case PhysicalType::UINT32:
		TemplatedLoopCombineHash<HAS_RSEL, uint32_t>(input, hashes, rsel, count);
		break;
	case PhysicalType::UINT64:
		TemplatedLoopCombineHash<HAS_RSEL, uint64_t>(input, hashes, rsel, count);
		break;
	case PhysicalType::INT128:
		TemplatedLoopCombineHash<HAS_RSEL, hugeint_t>(input, hashes, rsel, count);
		break;
	case PhysicalType::FLOAT:
		TemplatedLoopCombineHash<HAS_RSEL, float>(input, hashes, rsel, count);
		break;
	case PhysicalType::DOUBLE:
		TemplatedLoopCombineHash<HAS_RSEL, double>(input, hashes, rsel, count);
		break;
	case PhysicalType::INTERVAL:
		TemplatedLoopCombineHash<HAS_RSEL, interval_t>(input, hashes, rsel, count);
		break;
	case PhysicalType::VARCHAR:
		TemplatedLoopCombineHash<HAS_RSEL, string_t>(input, hashes, rsel, count);
		break;
	default:
		throw InvalidTypeException(input.GetType(), "Invalid type for hash");
	}
}

void VectorOperations::CombineHash(Vector &hashes, Vector &input, idx_t count) {
	CombineHashTypeSwitch<false>(hashes, input, nullptr, count);
}

void VectorOperations::CombineHash(Vector &hashes, Vector &input, const SelectionVector &rsel, idx_t count) {
	CombineHashTypeSwitch<true>(hashes, input, &rsel, count);
}

} // namespace guinsoodb

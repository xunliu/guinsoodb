//===--------------------------------------------------------------------===//
// comparison_operators.cpp
// Description: This file contains the implementation of the comparison
// operations == != >= <= > <
//===--------------------------------------------------------------------===//

#include "guinsoodb/common/operator/comparison_operators.hpp"

#include "guinsoodb/common/vector_operations/binary_executor.hpp"
#include "guinsoodb/common/vector_operations/vector_operations.hpp"

namespace guinsoodb {

struct ComparisonExecutor {
private:
	template <class T, class OP>
	static inline void TemplatedExecute(Vector &left, Vector &right, Vector &result, idx_t count) {
		BinaryExecutor::Execute<T, T, bool, OP>(left, right, result, count);
	}

public:
	template <class OP>
	static inline void Execute(Vector &left, Vector &right, Vector &result, idx_t count) {
		D_ASSERT(left.GetType() == right.GetType() && result.GetType() == LogicalType::BOOLEAN);
		// the inplace loops take the result as the last parameter
		switch (left.GetType().InternalType()) {
		case PhysicalType::BOOL:
		case PhysicalType::INT8:
			TemplatedExecute<int8_t, OP>(left, right, result, count);
			break;
		case PhysicalType::INT16:
			TemplatedExecute<int16_t, OP>(left, right, result, count);
			break;
		case PhysicalType::INT32:
			TemplatedExecute<int32_t, OP>(left, right, result, count);
			break;
		case PhysicalType::INT64:
			TemplatedExecute<int64_t, OP>(left, right, result, count);
			break;
		case PhysicalType::UINT8:
			TemplatedExecute<uint8_t, OP>(left, right, result, count);
			break;
		case PhysicalType::UINT16:
			TemplatedExecute<uint16_t, OP>(left, right, result, count);
			break;
		case PhysicalType::UINT32:
			TemplatedExecute<uint32_t, OP>(left, right, result, count);
			break;
		case PhysicalType::UINT64:
			TemplatedExecute<uint64_t, OP>(left, right, result, count);
			break;
		case PhysicalType::INT128:
			TemplatedExecute<hugeint_t, OP>(left, right, result, count);
			break;
		case PhysicalType::POINTER:
			TemplatedExecute<uintptr_t, OP>(left, right, result, count);
			break;
		case PhysicalType::FLOAT:
			TemplatedExecute<float, OP>(left, right, result, count);
			break;
		case PhysicalType::DOUBLE:
			TemplatedExecute<double, OP>(left, right, result, count);
			break;
		case PhysicalType::INTERVAL:
			TemplatedExecute<interval_t, OP>(left, right, result, count);
			break;
		case PhysicalType::VARCHAR:
			TemplatedExecute<string_t, OP>(left, right, result, count);
			break;
		default:
			throw InvalidTypeException(left.GetType(), "Invalid type for comparison");
		}
	}
};

void VectorOperations::Equals(Vector &left, Vector &right, Vector &result, idx_t count) {
	ComparisonExecutor::Execute<guinsoodb::Equals>(left, right, result, count);
}

void VectorOperations::NotEquals(Vector &left, Vector &right, Vector &result, idx_t count) {
	ComparisonExecutor::Execute<guinsoodb::NotEquals>(left, right, result, count);
}

void VectorOperations::GreaterThanEquals(Vector &left, Vector &right, Vector &result, idx_t count) {
	ComparisonExecutor::Execute<guinsoodb::GreaterThanEquals>(left, right, result, count);
}

void VectorOperations::LessThanEquals(Vector &left, Vector &right, Vector &result, idx_t count) {
	ComparisonExecutor::Execute<guinsoodb::LessThanEquals>(left, right, result, count);
}

void VectorOperations::GreaterThan(Vector &left, Vector &right, Vector &result, idx_t count) {
	ComparisonExecutor::Execute<guinsoodb::GreaterThan>(left, right, result, count);
}

void VectorOperations::LessThan(Vector &left, Vector &right, Vector &result, idx_t count) {
	ComparisonExecutor::Execute<guinsoodb::LessThan>(left, right, result, count);
}

} // namespace guinsoodb

#include "guinsoodb/common/types/hash.hpp"

#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/common/value_operations/value_operations.hpp"

namespace guinsoodb {

hash_t ValueOperations::Hash(const Value &op) {
	if (op.is_null) {
		return 0;
	}
	switch (op.type().InternalType()) {
	case PhysicalType::BOOL:
		return guinsoodb::Hash(op.value_.boolean);
	case PhysicalType::INT8:
		return guinsoodb::Hash(op.value_.tinyint);
	case PhysicalType::INT16:
		return guinsoodb::Hash(op.value_.smallint);
	case PhysicalType::INT32:
		return guinsoodb::Hash(op.value_.integer);
	case PhysicalType::INT64:
		return guinsoodb::Hash(op.value_.bigint);
	case PhysicalType::UINT8:
		return guinsoodb::Hash(op.value_.utinyint);
	case PhysicalType::UINT16:
		return guinsoodb::Hash(op.value_.usmallint);
	case PhysicalType::UINT32:
		return guinsoodb::Hash(op.value_.uinteger);
	case PhysicalType::UINT64:
		return guinsoodb::Hash(op.value_.ubigint);
	case PhysicalType::INT128:
		return guinsoodb::Hash(op.value_.hugeint);
	case PhysicalType::FLOAT:
		return guinsoodb::Hash(op.value_.float_);
	case PhysicalType::DOUBLE:
		return guinsoodb::Hash(op.value_.double_);
	case PhysicalType::POINTER:
		return guinsoodb::Hash(op.value_.pointer);
	case PhysicalType::INTERVAL:
		return guinsoodb::Hash(op.value_.interval);
	case PhysicalType::VARCHAR:
		return guinsoodb::Hash(op.str_value.c_str());
	case PhysicalType::LIST: {
		hash_t hash = 0;
		for (auto &entry : op.list_value) {
			hash ^= ValueOperations::Hash(entry);
		}
		return hash;
	}
	case PhysicalType::STRUCT: {
		hash_t hash = 0;
		for (auto &entry : op.struct_value) {
			hash ^= ValueOperations::Hash(entry.second);
		}
		return hash;
	}
	default:
		throw InternalException("Unimplemented type for value hash");
	}
}

} // namespace guinsoodb

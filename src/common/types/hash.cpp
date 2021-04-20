#include "guinsoodb/common/types/hash.hpp"
#include "guinsoodb/common/types/string_type.hpp"

#include "guinsoodb/common/exception.hpp"

#include <functional>

namespace guinsoodb {

template <>
hash_t Hash(uint64_t val) {
	return murmurhash64(val);
}

template <>
hash_t Hash(int64_t val) {
	return murmurhash64((uint64_t)val);
}

template <>
hash_t Hash(hugeint_t val) {
	return murmurhash64(val.lower) ^ murmurhash64(val.upper);
}

template <>
hash_t Hash(float val) {
	return std::hash<float> {}(val);
}

template <>
hash_t Hash(double val) {
	return std::hash<double> {}(val);
}

template <>
hash_t Hash(interval_t val) {
	return Hash(val.days) ^ Hash(val.months) ^ Hash(val.micros);
}

template <>
hash_t Hash(const char *str) {
	hash_t hash = 5381;
	hash_t c;

	while ((c = *str++)) {
		hash = ((hash << 5) + hash) + c;
	}

	return hash;
}

template <>
hash_t Hash(string_t val) {
	return Hash(val.GetDataUnsafe(), val.GetSize());
}

template <>
hash_t Hash(char *val) {
	return Hash<const char *>(val);
}

hash_t Hash(const char *val, size_t size) {
	hash_t hash = 5381;

	for (size_t i = 0; i < size; i++) {
		hash = ((hash << 5) + hash) + val[i];
	}

	return hash;
}

hash_t Hash(char *val, size_t size) {
	return Hash((const char *)val, size);
}

hash_t Hash(uint8_t *val, size_t size) {
	return Hash((const char *)val, size);
}

} // namespace guinsoodb

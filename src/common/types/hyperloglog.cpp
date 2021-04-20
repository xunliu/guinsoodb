#include "guinsoodb/common/types/hyperloglog.hpp"

#include "guinsoodb/common/exception.hpp"
#include "hyperloglog.hpp"

namespace guinsoodb {

HyperLogLog::HyperLogLog() : hll(nullptr) {
	hll = guinsoodb_hll::hll_create();
}

HyperLogLog::HyperLogLog(void *hll) : hll(hll) {
}

HyperLogLog::~HyperLogLog() {
	guinsoodb_hll::hll_destroy((guinsoodb_hll::robj *)hll);
}

void HyperLogLog::Add(data_ptr_t element, idx_t size) {
	if (guinsoodb_hll::hll_add((guinsoodb_hll::robj *)hll, element, size) == HLL_C_ERR) {
		throw Exception("Could not add to HLL?");
	}
}

idx_t HyperLogLog::Count() {
	size_t result; // exception from size_t ban
	if (guinsoodb_hll::hll_count((guinsoodb_hll::robj *)hll, &result) != HLL_C_OK) {
		throw Exception("Could not count HLL?");
	}
	return result;
}

unique_ptr<HyperLogLog> HyperLogLog::Merge(HyperLogLog &other) {
	guinsoodb_hll::robj *hlls[2];
	hlls[0] = (guinsoodb_hll::robj *)hll;
	hlls[1] = (guinsoodb_hll::robj *)other.hll;
	auto new_hll = guinsoodb_hll::hll_merge(hlls, 2);
	if (!new_hll) {
		throw Exception("Could not merge HLLs");
	}
	return unique_ptr<HyperLogLog>(new HyperLogLog((void *)new_hll));
}

HyperLogLog *HyperLogLog::MergePointer(HyperLogLog &other) {
	guinsoodb_hll::robj *hlls[2];
	hlls[0] = (guinsoodb_hll::robj *)hll;
	hlls[1] = (guinsoodb_hll::robj *)other.hll;
	auto new_hll = guinsoodb_hll::hll_merge(hlls, 2);
	if (!new_hll) {
		throw Exception("Could not merge HLLs");
	}
	return new HyperLogLog((void *)new_hll);
}

unique_ptr<HyperLogLog> HyperLogLog::Merge(HyperLogLog logs[], idx_t count) {
	auto hlls_uptr = unique_ptr<guinsoodb_hll::robj *[]> {
		new guinsoodb_hll::robj *[count]
	};
	auto hlls = hlls_uptr.get();
	for (idx_t i = 0; i < count; i++) {
		hlls[i] = (guinsoodb_hll::robj *)logs[i].hll;
	}
	auto new_hll = guinsoodb_hll::hll_merge(hlls, count);
	if (!new_hll) {
		throw Exception("Could not merge HLLs");
	}
	return unique_ptr<HyperLogLog>(new HyperLogLog((void *)new_hll));
}

} // namespace guinsoodb
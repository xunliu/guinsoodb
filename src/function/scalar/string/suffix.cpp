#include "guinsoodb/function/scalar/string_functions.hpp"
#include "guinsoodb/common/types/string_type.hpp"

#include "guinsoodb/common/exception.hpp"

namespace guinsoodb {

static bool SuffixFunction(const string_t &str, const string_t &suffix);

struct SuffixOperator {
	template <class TA, class TB, class TR>
	static inline TR Operation(TA left, TB right) {
		return SuffixFunction(left, right);
	}
};

static bool SuffixFunction(const string_t &str, const string_t &suffix) {
	auto suffix_size = suffix.GetSize();
	auto str_size = str.GetSize();
	if (suffix_size > str_size) {
		return false;
	}

	auto suffix_data = suffix.GetDataUnsafe();
	auto str_data = str.GetDataUnsafe();
	int32_t suf_idx = suffix_size - 1;
	idx_t str_idx = str_size - 1;
	for (; suf_idx >= 0; --suf_idx, --str_idx) {
		if (suffix_data[suf_idx] != str_data[str_idx]) {
			return false;
		}
	}
	return true;
}

ScalarFunction SuffixFun::GetFunction() {
	return ScalarFunction("suffix",                                     // name of the function
	                      {LogicalType::VARCHAR, LogicalType::VARCHAR}, // argument list
	                      LogicalType::BOOLEAN,                         // return type
	                      ScalarFunction::BinaryFunction<string_t, string_t, bool, SuffixOperator>);
}

void SuffixFun::RegisterFunction(BuiltinFunctions &set) {
	set.AddFunction(GetFunction());
}

} // namespace guinsoodb

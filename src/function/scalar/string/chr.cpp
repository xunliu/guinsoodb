#include "guinsoodb/function/scalar/string_functions.hpp"
#include "utf8proc.hpp"
#include "utf8proc_wrapper.hpp"

namespace guinsoodb {

struct ChrOperator {
	template <class TA, class TR>
	static inline TR Operation(const TA &input) {
		char c[5] = {'\0', '\0', '\0', '\0', '\0'};
		int utf8_bytes = 4;
		Utf8Proc::CodepointToUtf8(input, utf8_bytes, &c[0]);
		return string_t(&c[0]);
	}
};

void CHR::RegisterFunction(BuiltinFunctions &set) {
	ScalarFunction chr("chr", {LogicalType::INTEGER}, LogicalType::VARCHAR,
	                   ScalarFunction::UnaryFunction<int32_t, string_t, ChrOperator>);
	set.AddFunction(chr);
}

} // namespace guinsoodb

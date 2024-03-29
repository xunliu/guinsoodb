#include "guinsoodb/function/scalar/string_functions.hpp"

#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/common/vector_operations/vector_operations.hpp"
#include "guinsoodb/common/vector_operations/unary_executor.hpp"
#include "utf8proc.hpp"

#include <string.h>

namespace guinsoodb {

//! Fast ASCII string reverse, returns false if the input data is not ascii
static bool StrReverseASCII(const char *input, idx_t n, char *output) {
	for (idx_t i = 0; i < n; i++) {
		if (input[i] & 0x80) {
			// non-ascii character
			return false;
		}
		output[n - i - 1] = input[i];
	}
	return true;
}

//! Unicode string reverse using grapheme breakers
static void StrReverseUnicode(const char *input, idx_t n, char *output) {
	utf8proc_grapheme_callback(input, n, [&](size_t start, size_t end) {
		memcpy(output + n - end, input + start, end - start);
		return true;
	});
}

static void ReverseFunction(DataChunk &args, ExpressionState &state, Vector &result) {
	UnaryExecutor::Execute<string_t, string_t>(args.data[0], result, args.size(), [&](string_t input) {
		auto input_data = input.GetDataUnsafe();
		auto input_length = input.GetSize();

		auto target = StringVector::EmptyString(result, input_length);
		auto target_data = target.GetDataWriteable();
		if (!StrReverseASCII(input_data, input_length, target_data)) {
			StrReverseUnicode(input_data, input_length, target_data);
		}
		target.Finalize();
		return target;
	});
}

void ReverseFun::RegisterFunction(BuiltinFunctions &set) {
	set.AddFunction(ScalarFunction("reverse", {LogicalType::VARCHAR}, LogicalType::VARCHAR, ReverseFunction));
}

} // namespace guinsoodb

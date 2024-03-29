#include "guinsoodb/function/scalar/string_functions.hpp"

#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/common/crypto/md5.hpp"
#include "guinsoodb/common/vector_operations/unary_executor.hpp"

namespace guinsoodb {

static void MD5Function(DataChunk &args, ExpressionState &state, Vector &result) {
	auto &input = args.data[0];

	UnaryExecutor::Execute<string_t, string_t>(input, result, args.size(), [&](string_t input) {
		auto hash = StringVector::EmptyString(result, MD5Context::MD5_HASH_LENGTH_TEXT);
		MD5Context context;
		context.Add(input);
		context.FinishHex(hash.GetDataWriteable());
		hash.Finalize();
		return hash;
	});
}

void MD5Fun::RegisterFunction(BuiltinFunctions &set) {
	set.AddFunction(ScalarFunction("md5",                  // name of the function
	                               {LogicalType::VARCHAR}, // argument list
	                               LogicalType::VARCHAR,   // return type
	                               MD5Function));          // pointer to function implementation
}

} // namespace guinsoodb

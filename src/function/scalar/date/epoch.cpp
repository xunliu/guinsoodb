#include "guinsoodb/function/scalar/date_functions.hpp"
#include "guinsoodb/common/types/time.hpp"
#include "guinsoodb/common/types/date.hpp"
#include "guinsoodb/common/types/timestamp.hpp"
#include "guinsoodb/common/vector_operations/vector_operations.hpp"
#include "guinsoodb/common/vector_operations/unary_executor.hpp"

namespace guinsoodb {

static void EpochSecFunction(DataChunk &input, ExpressionState &state, Vector &result) {
	D_ASSERT(input.ColumnCount() == 1);

	string output_buffer;
	UnaryExecutor::Execute<int64_t, timestamp_t>(input.data[0], result, input.size(),
	                                             [&](int64_t input) { return Timestamp::FromEpochSeconds(input); });
}

static void EpochMillisFunction(DataChunk &input, ExpressionState &state, Vector &result) {
	D_ASSERT(input.ColumnCount() == 1);

	string output_buffer;
	UnaryExecutor::Execute<int64_t, timestamp_t>(input.data[0], result, input.size(),
	                                             [&](int64_t input) { return Timestamp::FromEpochMs(input); });
}

void EpochFun::RegisterFunction(BuiltinFunctions &set) {
	ScalarFunctionSet epoch("epoch_ms");
	epoch.AddFunction(ScalarFunction({LogicalType::BIGINT}, LogicalType::TIMESTAMP, EpochMillisFunction));
	set.AddFunction(epoch);
	// to_timestamp is an alias from Postgres that converts the time in seconds to a timestamp
	ScalarFunctionSet to_timestamp("to_timestamp");
	to_timestamp.AddFunction(ScalarFunction({LogicalType::BIGINT}, LogicalType::TIMESTAMP, EpochSecFunction));
	set.AddFunction(to_timestamp);
}

} // namespace guinsoodb

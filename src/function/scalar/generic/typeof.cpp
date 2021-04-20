#include "guinsoodb/function/scalar/generic_functions.hpp"

namespace guinsoodb {

static void TypeOfFunction(DataChunk &args, ExpressionState &state, Vector &result) {
	Value v(args.data[0].GetType().ToString());
	result.Reference(v);
}

void TypeOfFun::RegisterFunction(BuiltinFunctions &set) {
	set.AddFunction(ScalarFunction("typeof", {LogicalType::ANY}, LogicalType::VARCHAR, TypeOfFunction));
}

} // namespace guinsoodb

#include "guinsoodb/function/udf_function.hpp"

#include "guinsoodb/parser/parsed_data/create_scalar_function_info.hpp"
#include "guinsoodb/parser/parsed_data/create_aggregate_function_info.hpp"

#include "guinsoodb/main/client_context.hpp"

namespace guinsoodb {

void UDFWrapper::RegisterFunction(string name, vector<LogicalType> args, LogicalType ret_type,
                                  scalar_function_t udf_function, ClientContext &context, LogicalType varargs) {

	ScalarFunction scalar_function(move(name), move(args), move(ret_type), move(udf_function));
	scalar_function.varargs = move(varargs);
	CreateScalarFunctionInfo info(scalar_function);
	info.schema = DEFAULT_SCHEMA;
	context.RegisterFunction(&info);
}

void UDFWrapper::RegisterAggrFunction(AggregateFunction aggr_function, ClientContext &context, LogicalType varargs) {
	aggr_function.varargs = move(varargs);
	CreateAggregateFunctionInfo info(move(aggr_function));
	context.RegisterFunction(&info);
}

} // namespace guinsoodb

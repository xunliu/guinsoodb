//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb_python/pandas_scan.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb.hpp"
#include "guinsoodb_python/pybind_wrapper.hpp"
#include "guinsoodb/parser/parsed_data/create_table_function_info.hpp"

namespace guinsoodb {

struct MapFunction : public TableFunction {

public:
	MapFunction();

	static unique_ptr<FunctionData> MapFunctionBind(ClientContext &context, vector<Value> &inputs,
	                                                unordered_map<string, Value> &named_parameters,
	                                                vector<LogicalType> &input_table_types,
	                                                vector<string> &input_table_names,
	                                                vector<LogicalType> &return_types, vector<string> &names);

	static void MapFunctionExec(ClientContext &context, const FunctionData *bind_data,
	                            FunctionOperatorData *operator_state, DataChunk *input, DataChunk &output);
};

} // namespace guinsoodb

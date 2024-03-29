//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/function/copy_function.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/function/function.hpp"
#include "guinsoodb/function/table_function.hpp"
#include "guinsoodb/parser/parsed_data/copy_info.hpp"

namespace guinsoodb {
class ExecutionContext;

struct LocalFunctionData {
	virtual ~LocalFunctionData() {
	}
};

struct GlobalFunctionData {
	virtual ~GlobalFunctionData() {
	}
};

typedef unique_ptr<FunctionData> (*copy_to_bind_t)(ClientContext &context, CopyInfo &info, vector<string> &names,
                                                   vector<LogicalType> &sql_types);
typedef unique_ptr<LocalFunctionData> (*copy_to_initialize_local_t)(ClientContext &context, FunctionData &bind_data);
typedef unique_ptr<GlobalFunctionData> (*copy_to_initialize_global_t)(ClientContext &context, FunctionData &bind_data);
typedef void (*copy_to_sink_t)(ClientContext &context, FunctionData &bind_data, GlobalFunctionData &gstate,
                               LocalFunctionData &lstate, DataChunk &input);
typedef void (*copy_to_combine_t)(ClientContext &context, FunctionData &bind_data, GlobalFunctionData &gstate,
                                  LocalFunctionData &lstate);
typedef void (*copy_to_finalize_t)(ClientContext &context, FunctionData &bind_data, GlobalFunctionData &gstate);

typedef unique_ptr<FunctionData> (*copy_from_bind_t)(ClientContext &context, CopyInfo &info,
                                                     vector<string> &expected_names,
                                                     vector<LogicalType> &expected_types);

class CopyFunction : public Function {
public:
	explicit CopyFunction(string name)
	    : Function(name), copy_to_bind(nullptr), copy_to_initialize_local(nullptr), copy_to_initialize_global(nullptr),
	      copy_to_sink(nullptr), copy_to_combine(nullptr), copy_to_finalize(nullptr), copy_from_bind(nullptr) {
	}

	copy_to_bind_t copy_to_bind;
	copy_to_initialize_local_t copy_to_initialize_local;
	copy_to_initialize_global_t copy_to_initialize_global;
	copy_to_sink_t copy_to_sink;
	copy_to_combine_t copy_to_combine;
	copy_to_finalize_t copy_to_finalize;

	copy_from_bind_t copy_from_bind;
	TableFunction copy_from_function;

	string extension;
};

} // namespace guinsoodb

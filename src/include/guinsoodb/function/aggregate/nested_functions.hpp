//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/function/aggregate/nested_functions.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/function/aggregate_function.hpp"
#include "guinsoodb/function/function_set.hpp"

namespace guinsoodb {

struct ListBindData : public FunctionData {
	ListBindData() {
	}

	unique_ptr<FunctionData> Copy() override {
		return make_unique<ListBindData>();
	}
};

struct ListFun {
	static void RegisterFunction(BuiltinFunctions &set);
};
struct HistogramFun {
	static void RegisterFunction(BuiltinFunctions &set);
};
} // namespace guinsoodb

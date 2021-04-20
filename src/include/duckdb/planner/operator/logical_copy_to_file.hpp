//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/operator/logical_copy_to_file.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/planner/logical_operator.hpp"
#include "guinsoodb/function/copy_function.hpp"

namespace guinsoodb {

class LogicalCopyToFile : public LogicalOperator {
public:
	LogicalCopyToFile(CopyFunction function, unique_ptr<FunctionData> bind_data)
	    : LogicalOperator(LogicalOperatorType::LOGICAL_COPY_TO_FILE), function(function), bind_data(move(bind_data)) {
	}
	CopyFunction function;
	unique_ptr<FunctionData> bind_data;

protected:
	void ResolveTypes() override {
		types.push_back(LogicalType::BIGINT);
	}
};
} // namespace guinsoodb

//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/operator/logical_export.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_data/copy_info.hpp"
#include "guinsoodb/planner/logical_operator.hpp"
#include "guinsoodb/function/copy_function.hpp"

namespace guinsoodb {

class LogicalExport : public LogicalOperator {
public:
	LogicalExport(CopyFunction function, unique_ptr<CopyInfo> copy_info)
	    : LogicalOperator(LogicalOperatorType::LOGICAL_EXPORT), function(function), copy_info(move(copy_info)) {
	}
	CopyFunction function;
	unique_ptr<CopyInfo> copy_info;

protected:
	void ResolveTypes() override {
		types.push_back(LogicalType::BOOLEAN);
	}
};

} // namespace guinsoodb

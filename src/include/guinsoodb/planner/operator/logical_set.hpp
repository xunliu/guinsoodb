//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/operator/logical_set.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_data/copy_info.hpp"
#include "guinsoodb/planner/logical_operator.hpp"
#include "guinsoodb/function/copy_function.hpp"

namespace guinsoodb {

class LogicalSet : public LogicalOperator {
public:
	LogicalSet(std::string name_p, Value value_p)
	    : LogicalOperator(LogicalOperatorType::LOGICAL_SET), name(name_p), value(value_p) {
	}
	std::string name;
	Value value;

protected:
	void ResolveTypes() override {
		types.push_back(LogicalType::BOOLEAN);
	}
};

} // namespace guinsoodb

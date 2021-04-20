//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/operator/logical_simple.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/enums/statement_type.hpp"
#include "guinsoodb/parser/parsed_data/parse_info.hpp"
#include "guinsoodb/planner/logical_operator.hpp"

namespace guinsoodb {

//! LogicalSimple represents a simple logical operator that only passes on the parse info
class LogicalSimple : public LogicalOperator {
public:
	LogicalSimple(LogicalOperatorType type, unique_ptr<ParseInfo> info) : LogicalOperator(type), info(move(info)) {
	}

	unique_ptr<ParseInfo> info;

protected:
	void ResolveTypes() override {
		types.push_back(LogicalType::BOOLEAN);
	}
};
} // namespace guinsoodb

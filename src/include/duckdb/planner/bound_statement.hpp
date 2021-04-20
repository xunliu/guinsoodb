//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/bound_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/planner/logical_operator.hpp"

namespace guinsoodb {

struct BoundStatement {
	unique_ptr<LogicalOperator> plan;
	vector<LogicalType> types;
	vector<string> names;
};

} // namespace guinsoodb

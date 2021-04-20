//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/optimizer/regex_range_filter.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/optimizer/rule.hpp"

namespace guinsoodb {

class Optimizer;

class RegexRangeFilter {
public:
	RegexRangeFilter() {
	}
	//! Perform filter pushdown
	unique_ptr<LogicalOperator> Rewrite(unique_ptr<LogicalOperator> op);
};

} // namespace guinsoodb

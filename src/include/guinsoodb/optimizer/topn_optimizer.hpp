//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/optimizer/topn_optimizer.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/constants.hpp"

namespace guinsoodb {
class LogicalOperator;
class Optimizer;

class TopN {
public:
	//! Optimize ORDER BY + LIMIT to TopN
	unique_ptr<LogicalOperator> Optimize(unique_ptr<LogicalOperator> op);
};

} // namespace guinsoodb

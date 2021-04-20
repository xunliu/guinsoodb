//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/operator/logical_prepare.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/unordered_map.hpp"
#include "guinsoodb/common/unordered_set.hpp"
#include "guinsoodb/main/prepared_statement_data.hpp"
#include "guinsoodb/planner/logical_operator.hpp"

namespace guinsoodb {

class TableCatalogEntry;

class LogicalPrepare : public LogicalOperator {
public:
	LogicalPrepare(string name, shared_ptr<PreparedStatementData> prepared, unique_ptr<LogicalOperator> logical_plan)
	    : LogicalOperator(LogicalOperatorType::LOGICAL_PREPARE), name(name), prepared(move(prepared)) {
		children.push_back(move(logical_plan));
	}

	string name;
	shared_ptr<PreparedStatementData> prepared;

protected:
	void ResolveTypes() override {
		types.push_back(LogicalType::BOOLEAN);
	}
};
} // namespace guinsoodb

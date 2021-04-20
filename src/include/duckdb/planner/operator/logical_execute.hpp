//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/operator/logical_execute.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/main/prepared_statement_data.hpp"
#include "guinsoodb/planner/logical_operator.hpp"

namespace guinsoodb {

class LogicalExecute : public LogicalOperator {
public:
	explicit LogicalExecute(shared_ptr<PreparedStatementData> prepared_p)
	    : LogicalOperator(LogicalOperatorType::LOGICAL_EXECUTE), prepared(move(prepared_p)) {
		D_ASSERT(prepared);
		types = prepared->types;
	}

	shared_ptr<PreparedStatementData> prepared;

protected:
	void ResolveTypes() override {
		// already resolved
	}
	vector<ColumnBinding> GetColumnBindings() override {
		vector<ColumnBinding> bindings;
		for (idx_t i = 0; i < types.size(); i++) {
			bindings.push_back(ColumnBinding(0, i));
		}
		return bindings;
	}
};
} // namespace guinsoodb

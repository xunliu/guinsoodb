#include "guinsoodb/planner/operator/logical_distinct.hpp"
#include "guinsoodb/common/string_util.hpp"

namespace guinsoodb {

string LogicalDistinct::ParamsToString() const {
	string result = LogicalOperator::ParamsToString();
	if (!distinct_targets.empty()) {
		result += StringUtil::Join(distinct_targets, distinct_targets.size(), "\n",
		                           [](const unique_ptr<Expression> &child) { return child->GetName(); });
	}

	return result;
}

} // namespace guinsoodb

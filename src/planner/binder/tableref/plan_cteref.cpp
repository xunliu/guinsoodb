#include "guinsoodb/planner/binder.hpp"
#include "guinsoodb/planner/operator/logical_cteref.hpp"
#include "guinsoodb/planner/tableref/bound_cteref.hpp"

namespace guinsoodb {

unique_ptr<LogicalOperator> Binder::CreatePlan(BoundCTERef &ref) {
	auto index = ref.bind_index;

	vector<LogicalType> types;
	for (auto &type : ref.types) {
		types.push_back(type);
	}

	return make_unique<LogicalCTERef>(index, ref.cte_index, types, ref.bound_columns);
}

} // namespace guinsoodb

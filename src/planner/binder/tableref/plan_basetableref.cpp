#include "guinsoodb/planner/binder.hpp"
#include "guinsoodb/planner/operator/logical_get.hpp"
#include "guinsoodb/planner/tableref/bound_basetableref.hpp"

namespace guinsoodb {

unique_ptr<LogicalOperator> Binder::CreatePlan(BoundBaseTableRef &ref) {
	return move(ref.get);
}

} // namespace guinsoodb

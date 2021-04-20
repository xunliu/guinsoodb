#include "guinsoodb/planner/binder.hpp"
#include "guinsoodb/planner/operator/logical_dummy_scan.hpp"
#include "guinsoodb/planner/tableref/bound_dummytableref.hpp"

namespace guinsoodb {

unique_ptr<LogicalOperator> Binder::CreatePlan(BoundEmptyTableRef &ref) {
	return make_unique<LogicalDummyScan>(ref.bind_index);
}

} // namespace guinsoodb

#include "guinsoodb/planner/binder.hpp"
#include "guinsoodb/planner/tableref/bound_table_function.hpp"

namespace guinsoodb {

unique_ptr<LogicalOperator> Binder::CreatePlan(BoundTableFunction &ref) {
	return move(ref.get);
}

} // namespace guinsoodb

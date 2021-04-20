#include "guinsoodb/planner/binder.hpp"
#include "guinsoodb/parser/statement/call_statement.hpp"
#include "guinsoodb/parser/tableref/table_function_ref.hpp"
#include "guinsoodb/planner/tableref/bound_table_function.hpp"
#include "guinsoodb/planner/operator/logical_get.hpp"

namespace guinsoodb {

BoundStatement Binder::Bind(CallStatement &stmt) {
	BoundStatement result;

	TableFunctionRef ref;
	ref.function = move(stmt.function);

	auto bound_func = Bind(ref);
	auto &bound_table_func = (BoundTableFunction &)*bound_func;
	auto &get = (LogicalGet &)*bound_table_func.get;
	D_ASSERT(get.returned_types.size() > 0);
	for (idx_t i = 0; i < get.returned_types.size(); i++) {
		get.column_ids.push_back(i);
	}

	result.types = get.returned_types;
	result.names = get.names;
	result.plan = CreatePlan(*bound_func);
	return result;
}

} // namespace guinsoodb

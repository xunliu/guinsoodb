#include "guinsoodb/optimizer/filter_pushdown.hpp"
#include "guinsoodb/optimizer/optimizer.hpp"
#include "guinsoodb/planner/expression/bound_columnref_expression.hpp"
#include "guinsoodb/planner/operator/logical_filter.hpp"
#include "guinsoodb/planner/operator/logical_get.hpp"
#include "guinsoodb/storage/data_table.hpp"

namespace guinsoodb {

unique_ptr<LogicalOperator> FilterPushdown::PushdownGet(unique_ptr<LogicalOperator> op) {
	D_ASSERT(op->type == LogicalOperatorType::LOGICAL_GET);
	auto &get = (LogicalGet &)*op;

	if (get.function.pushdown_complex_filter) {
		// for the remaining filters, check if we can push any of them into the scan as well
		vector<unique_ptr<Expression>> expressions;
		for (auto &filter : filters) {
			expressions.push_back(move(filter->filter));
		}
		filters.clear();

		get.function.pushdown_complex_filter(optimizer.context, get, get.bind_data.get(), expressions);

		if (expressions.empty()) {
			return op;
		}
		// re-generate the filters
		for (auto &expr : expressions) {
			auto f = make_unique<Filter>();
			f->filter = move(expr);
			f->ExtractBindings();
			filters.push_back(move(f));
		}
	}
	if (!get.table_filters.empty() || !get.function.filter_pushdown) {
		// the table function does not support filter pushdown: push a LogicalFilter on top
		return FinishPushdown(move(op));
	}
	PushFilters();

	//! We generate the table filters that will be executed during the table scan
	//! Right now this only executes simple AND filters
	get.table_filters = combiner.GenerateTableScanFilters(get.column_ids);

	//! For more complex filters if all filters to a column are constants we generate a min max boundary used to check
	//! the zonemaps.
	auto zonemap_checks = combiner.GenerateZonemapChecks(get.column_ids, get.table_filters);

	for (auto &f : get.table_filters) {
		f.column_index = get.column_ids[f.column_index];
	}

	//! Use zonemap checks as table filters for pre-processing
	for (auto &zonemap_check : zonemap_checks) {
		if (zonemap_check.column_index != COLUMN_IDENTIFIER_ROW_ID) {
			get.table_filters.push_back(zonemap_check);
		}
	}

	GenerateFilters();

	//! Now we try to pushdown the remaining filters to perform zonemap checking
	return FinishPushdown(move(op));
}

} // namespace guinsoodb

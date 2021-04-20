#include "guinsoodb/parser/statement/delete_statement.hpp"
#include "guinsoodb/planner/binder.hpp"
#include "guinsoodb/planner/expression_binder/where_binder.hpp"
#include "guinsoodb/planner/operator/logical_delete.hpp"
#include "guinsoodb/planner/operator/logical_filter.hpp"
#include "guinsoodb/planner/operator/logical_get.hpp"
#include "guinsoodb/planner/bound_tableref.hpp"
#include "guinsoodb/planner/tableref/bound_basetableref.hpp"

namespace guinsoodb {

BoundStatement Binder::Bind(DeleteStatement &stmt) {
	BoundStatement result;

	// visit the table reference
	auto bound_table = Bind(*stmt.table);
	if (bound_table->type != TableReferenceType::BASE_TABLE) {
		throw BinderException("Can only delete from base table!");
	}
	auto &table_binding = (BoundBaseTableRef &)*bound_table;
	auto table = table_binding.table;

	auto root = CreatePlan(*bound_table);
	auto &get = (LogicalGet &)*root;
	D_ASSERT(root->type == LogicalOperatorType::LOGICAL_GET);

	if (!table->temporary) {
		// delete from persistent table: not read only!
		this->read_only = false;
	}
	// project any additional columns required for the condition
	unique_ptr<Expression> condition;
	if (stmt.condition) {
		WhereBinder binder(*this, context);
		condition = binder.Bind(stmt.condition);

		PlanSubqueries(&condition, &root);
		auto filter = make_unique<LogicalFilter>(move(condition));
		filter->AddChild(move(root));
		root = move(filter);
	}
	// create the delete node
	auto del = make_unique<LogicalDelete>(table);
	del->AddChild(move(root));

	// set up the delete expression
	del->expressions.push_back(
	    make_unique<BoundColumnRefExpression>(LOGICAL_ROW_TYPE, ColumnBinding(get.table_index, get.column_ids.size())));
	get.column_ids.push_back(COLUMN_IDENTIFIER_ROW_ID);

	result.plan = move(del);
	result.names = {"Count"};
	result.types = {LogicalType::BIGINT};
	return result;
}

} // namespace guinsoodb

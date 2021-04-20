#include "guinsoodb/planner/operator/logical_projection.hpp"

namespace guinsoodb {

LogicalProjection::LogicalProjection(idx_t table_index, vector<unique_ptr<Expression>> select_list)
    : LogicalOperator(LogicalOperatorType::LOGICAL_PROJECTION, move(select_list)), table_index(table_index) {
}

vector<ColumnBinding> LogicalProjection::GetColumnBindings() {
	return GenerateColumnBindings(table_index, expressions.size());
}

void LogicalProjection::ResolveTypes() {
	for (auto &expr : expressions) {
		types.push_back(expr->return_type);
	}
}

} // namespace guinsoodb

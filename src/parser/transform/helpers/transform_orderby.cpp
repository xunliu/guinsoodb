#include "guinsoodb/parser/expression/constant_expression.hpp"
#include "guinsoodb/parser/statement/select_statement.hpp"
#include "guinsoodb/parser/transformer.hpp"

namespace guinsoodb {

bool Transformer::TransformOrderBy(guinsoodb_libpgquery::PGList *order, vector<OrderByNode> &result) {
	if (!order) {
		return false;
	}

	for (auto node = order->head; node != nullptr; node = node->next) {
		auto temp = reinterpret_cast<guinsoodb_libpgquery::PGNode *>(node->data.ptr_value);
		if (temp->type == guinsoodb_libpgquery::T_PGSortBy) {
			OrderType type;
			OrderByNullType null_order;
			auto sort = reinterpret_cast<guinsoodb_libpgquery::PGSortBy *>(temp);
			auto target = sort->node;
			if (sort->sortby_dir == guinsoodb_libpgquery::PG_SORTBY_DEFAULT) {
				type = OrderType::ORDER_DEFAULT;
			} else if (sort->sortby_dir == guinsoodb_libpgquery::PG_SORTBY_ASC) {
				type = OrderType::ASCENDING;
			} else if (sort->sortby_dir == guinsoodb_libpgquery::PG_SORTBY_DESC) {
				type = OrderType::DESCENDING;
			} else {
				throw NotImplementedException("Unimplemented order by type");
			}
			if (sort->sortby_nulls == guinsoodb_libpgquery::PG_SORTBY_NULLS_DEFAULT) {
				null_order = OrderByNullType::ORDER_DEFAULT;
			} else if (sort->sortby_nulls == guinsoodb_libpgquery::PG_SORTBY_NULLS_FIRST) {
				null_order = OrderByNullType::NULLS_FIRST;
			} else if (sort->sortby_nulls == guinsoodb_libpgquery::PG_SORTBY_NULLS_LAST) {
				null_order = OrderByNullType::NULLS_LAST;
			} else {
				throw NotImplementedException("Unimplemented order by type");
			}
			auto order_expression = TransformExpression(target);
			result.emplace_back(type, null_order, move(order_expression));
		} else {
			throw NotImplementedException("ORDER BY list member type %d\n", temp->type);
		}
	}
	return true;
}

} // namespace guinsoodb

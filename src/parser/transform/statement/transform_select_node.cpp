#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/parser/query_node/select_node.hpp"
#include "guinsoodb/parser/query_node/set_operation_node.hpp"
#include "guinsoodb/parser/statement/select_statement.hpp"
#include "guinsoodb/parser/transformer.hpp"
#include "guinsoodb/parser/expression/star_expression.hpp"
#include "guinsoodb/common/string_util.hpp"

namespace guinsoodb {

unique_ptr<QueryNode> Transformer::TransformSelectNode(guinsoodb_libpgquery::PGSelectStmt *stmt) {
	unique_ptr<QueryNode> node;

	switch (stmt->op) {
	case guinsoodb_libpgquery::PG_SETOP_NONE: {
		node = make_unique<SelectNode>();
		auto result = (SelectNode *)node.get();
		if (stmt->withClause) {
			TransformCTE(reinterpret_cast<guinsoodb_libpgquery::PGWithClause *>(stmt->withClause), *node);
		}
		if (stmt->windowClause) {
			for (auto window_ele = stmt->windowClause->head; window_ele != nullptr; window_ele = window_ele->next) {
				auto window_def = reinterpret_cast<guinsoodb_libpgquery::PGWindowDef *>(window_ele->data.ptr_value);
				D_ASSERT(window_def);
				D_ASSERT(window_def->name);
				auto window_name = StringUtil::Lower(string(window_def->name));

				auto it = window_clauses.find(window_name);
				if (it != window_clauses.end()) {
					throw ParserException("window \"%s\" is already defined", window_name);
				}
				window_clauses[window_name] = window_def;
			}
		}

		// checks distinct clause
		if (stmt->distinctClause != nullptr) {
			auto modifier = make_unique<DistinctModifier>();
			// checks distinct on clause
			auto target = reinterpret_cast<guinsoodb_libpgquery::PGNode *>(stmt->distinctClause->head->data.ptr_value);
			if (target) {
				//  add the columns defined in the ON clause to the select list
				if (!TransformExpressionList(stmt->distinctClause, modifier->distinct_on_targets)) {
					throw Exception("Failed to transform expression list from DISTINCT ON.");
				}
			}
			result->modifiers.push_back(move(modifier));
		}

		// do this early so the value lists also have a `FROM`
		if (stmt->valuesLists) {
			// VALUES list, create an ExpressionList
			D_ASSERT(!stmt->fromClause);
			result->from_table = TransformValuesList(stmt->valuesLists);
			result->select_list.push_back(make_unique<StarExpression>());
		} else {
			if (!stmt->targetList) {
				throw ParserException("SELECT clause without selection list");
			}
			// select list
			if (!TransformExpressionList(stmt->targetList, result->select_list)) {
				throw InternalException("Failed to transform expression list.");
			}
			result->from_table = TransformFrom(stmt->fromClause);
		}

		// where
		result->where_clause = TransformExpression(stmt->whereClause);
		// group by
		TransformGroupBy(stmt->groupClause, result->groups);
		// having
		result->having = TransformExpression(stmt->havingClause);
		// sample
		result->sample = TransformSampleOptions(stmt->sampleOptions);
		break;
	}
	case guinsoodb_libpgquery::PG_SETOP_UNION:
	case guinsoodb_libpgquery::PG_SETOP_EXCEPT:
	case guinsoodb_libpgquery::PG_SETOP_INTERSECT: {
		node = make_unique<SetOperationNode>();
		auto result = (SetOperationNode *)node.get();
		if (stmt->withClause) {
			TransformCTE(reinterpret_cast<guinsoodb_libpgquery::PGWithClause *>(stmt->withClause), *node);
		}
		result->left = TransformSelectNode(stmt->larg);
		result->right = TransformSelectNode(stmt->rarg);
		if (!result->left || !result->right) {
			throw Exception("Failed to transform setop children.");
		}

		bool select_distinct = true;
		switch (stmt->op) {
		case guinsoodb_libpgquery::PG_SETOP_UNION:
			select_distinct = !stmt->all;
			result->setop_type = SetOperationType::UNION;
			break;
		case guinsoodb_libpgquery::PG_SETOP_EXCEPT:
			result->setop_type = SetOperationType::EXCEPT;
			break;
		case guinsoodb_libpgquery::PG_SETOP_INTERSECT:
			result->setop_type = SetOperationType::INTERSECT;
			break;
		default:
			throw Exception("Unexpected setop type");
		}
		if (select_distinct) {
			result->modifiers.push_back(make_unique<DistinctModifier>());
		}
		if (stmt->sampleOptions) {
			throw ParserException("SAMPLE clause is only allowed in regular SELECT statements");
		}
		break;
	}
	default:
		throw NotImplementedException("Statement type %d not implemented!", stmt->op);
	}
	// transform the common properties
	// both the set operations and the regular select can have an ORDER BY/LIMIT attached to them
	vector<OrderByNode> orders;
	TransformOrderBy(stmt->sortClause, orders);
	if (!orders.empty()) {
		auto order_modifier = make_unique<OrderModifier>();
		order_modifier->orders = move(orders);
		node->modifiers.push_back(move(order_modifier));
	}
	if (stmt->limitCount || stmt->limitOffset) {
		auto limit_modifier = make_unique<LimitModifier>();
		if (stmt->limitCount) {
			limit_modifier->limit = TransformExpression(stmt->limitCount);
		}
		if (stmt->limitOffset) {
			limit_modifier->offset = TransformExpression(stmt->limitOffset);
		}
		node->modifiers.push_back(move(limit_modifier));
	}
	return node;
}

} // namespace guinsoodb

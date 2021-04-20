#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/parser/expression/columnref_expression.hpp"
#include "guinsoodb/parser/expression/star_expression.hpp"
#include "guinsoodb/parser/expression/table_star_expression.hpp"
#include "guinsoodb/parser/transformer.hpp"

namespace guinsoodb {

unique_ptr<ParsedExpression> Transformer::TransformColumnRef(guinsoodb_libpgquery::PGColumnRef *root) {
	auto fields = root->fields;
	switch ((reinterpret_cast<guinsoodb_libpgquery::PGNode *>(fields->head->data.ptr_value))->type) {
	case guinsoodb_libpgquery::T_PGString: {
		if (fields->length < 1) {
			throw ParserException("Unexpected field length");
		}
		string column_name, table_name;
		if (fields->length == 1) {
			column_name = string(reinterpret_cast<guinsoodb_libpgquery::PGValue *>(fields->head->data.ptr_value)->val.str);
			auto colref = make_unique<ColumnRefExpression>(column_name, table_name);
			colref->query_location = root->location;
			return move(colref);
		} else if (fields->length == 2) {
			table_name = string(reinterpret_cast<guinsoodb_libpgquery::PGValue *>(fields->head->data.ptr_value)->val.str);
			auto col_node = reinterpret_cast<guinsoodb_libpgquery::PGNode *>(fields->head->next->data.ptr_value);
			switch (col_node->type) {
			case guinsoodb_libpgquery::T_PGString: {
				column_name = string(reinterpret_cast<guinsoodb_libpgquery::PGValue *>(col_node)->val.str);
				auto colref = make_unique<ColumnRefExpression>(column_name, table_name);
				colref->query_location = root->location;
				return move(colref);
			}
			case guinsoodb_libpgquery::T_PGAStar: {
				return make_unique<TableStarExpression>(table_name);
			}
			default:
				throw NotImplementedException("ColumnRef not implemented!");
			}
		} else {
			throw NotImplementedException("ColumnRef not implemented!");
		}
	}
	case guinsoodb_libpgquery::T_PGAStar: {
		return make_unique<StarExpression>();
	}
	default:
		break;
	}
	throw NotImplementedException("ColumnRef not implemented!");
}

} // namespace guinsoodb

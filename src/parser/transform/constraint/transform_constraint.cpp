#include "guinsoodb/parser/column_definition.hpp"
#include "guinsoodb/parser/constraint.hpp"
#include "guinsoodb/parser/constraints/list.hpp"
#include "guinsoodb/parser/transformer.hpp"

namespace guinsoodb {

unique_ptr<Constraint> Transformer::TransformConstraint(guinsoodb_libpgquery::PGListCell *cell) {
	auto constraint = reinterpret_cast<guinsoodb_libpgquery::PGConstraint *>(cell->data.ptr_value);
	switch (constraint->contype) {
	case guinsoodb_libpgquery::PG_CONSTR_UNIQUE:
	case guinsoodb_libpgquery::PG_CONSTR_PRIMARY: {
		bool is_primary_key = constraint->contype == guinsoodb_libpgquery::PG_CONSTR_PRIMARY;
		vector<string> columns;
		for (auto kc = constraint->keys->head; kc; kc = kc->next) {
			columns.emplace_back(reinterpret_cast<guinsoodb_libpgquery::PGValue *>(kc->data.ptr_value)->val.str);
		}
		return make_unique<UniqueConstraint>(columns, is_primary_key);
	}
	case guinsoodb_libpgquery::PG_CONSTR_CHECK: {
		auto expression = TransformExpression(constraint->raw_expr);
		if (expression->HasSubquery()) {
			throw ParserException("subqueries prohibited in CHECK constraints");
		}
		if (expression->IsAggregate()) {
			throw ParserException("aggregates prohibited in CHECK constraints");
		}
		return make_unique<CheckConstraint>(TransformExpression(constraint->raw_expr));
	}
	default:
		throw NotImplementedException("Constraint type not handled yet!");
	}
}

unique_ptr<Constraint> Transformer::TransformConstraint(guinsoodb_libpgquery::PGListCell *cell, ColumnDefinition &column,
                                                        idx_t index) {
	auto constraint = reinterpret_cast<guinsoodb_libpgquery::PGConstraint *>(cell->data.ptr_value);
	D_ASSERT(constraint);
	switch (constraint->contype) {
	case guinsoodb_libpgquery::PG_CONSTR_NOTNULL:
		return make_unique<NotNullConstraint>(index);
	case guinsoodb_libpgquery::PG_CONSTR_CHECK:
		return TransformConstraint(cell);
	case guinsoodb_libpgquery::PG_CONSTR_PRIMARY:
		return make_unique<UniqueConstraint>(index, true);
	case guinsoodb_libpgquery::PG_CONSTR_UNIQUE:
		return make_unique<UniqueConstraint>(index, false);
	case guinsoodb_libpgquery::PG_CONSTR_NULL:
		return nullptr;
	case guinsoodb_libpgquery::PG_CONSTR_DEFAULT:
		column.default_value = TransformExpression(constraint->raw_expr);
		return nullptr;
	case guinsoodb_libpgquery::PG_CONSTR_FOREIGN:
	default:
		throw NotImplementedException("Constraint not implemented!");
	}
}

} // namespace guinsoodb

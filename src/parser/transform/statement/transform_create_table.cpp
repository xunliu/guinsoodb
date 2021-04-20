#include "guinsoodb/parser/statement/create_statement.hpp"
#include "guinsoodb/parser/parsed_data/create_table_info.hpp"
#include "guinsoodb/parser/transformer.hpp"
#include "guinsoodb/parser/constraint.hpp"
#include "guinsoodb/parser/expression/collate_expression.hpp"

namespace guinsoodb {

string Transformer::TransformCollation(guinsoodb_libpgquery::PGCollateClause *collate) {
	if (!collate) {
		return string();
	}
	string collation;
	for (auto c = collate->collname->head; c != nullptr; c = lnext(c)) {
		auto pgvalue = (guinsoodb_libpgquery::PGValue *)c->data.ptr_value;
		if (pgvalue->type != guinsoodb_libpgquery::T_PGString) {
			throw ParserException("Expected a string as collation type!");
		}
		auto collation_argument = string(pgvalue->val.str);
		if (collation.empty()) {
			collation = collation_argument;
		} else {
			collation += "." + collation_argument;
		}
	}
	return collation;
}

unique_ptr<ParsedExpression> Transformer::TransformCollateExpr(guinsoodb_libpgquery::PGCollateClause *collate) {
	auto child = TransformExpression(collate->arg);
	auto collation = TransformCollation(collate);
	return make_unique<CollateExpression>(collation, move(child));
}

ColumnDefinition Transformer::TransformColumnDefinition(guinsoodb_libpgquery::PGColumnDef *cdef) {
	string colname;
	if (cdef->colname) {
		colname = cdef->colname;
	}
	LogicalType target_type = TransformTypeName(cdef->typeName);
	if (cdef->collClause) {
		if (target_type.id() != LogicalTypeId::VARCHAR) {
			throw ParserException("Only VARCHAR columns can have collations!");
		}
		target_type = LogicalType(LogicalTypeId::VARCHAR, TransformCollation(cdef->collClause));
	}

	return ColumnDefinition(colname, target_type);
}

unique_ptr<CreateStatement> Transformer::TransformCreateTable(guinsoodb_libpgquery::PGNode *node) {
	auto stmt = reinterpret_cast<guinsoodb_libpgquery::PGCreateStmt *>(node);
	D_ASSERT(stmt);
	auto result = make_unique<CreateStatement>();
	auto info = make_unique<CreateTableInfo>();

	if (stmt->inhRelations) {
		throw NotImplementedException("inherited relations not implemented");
	}
	D_ASSERT(stmt->relation);

	info->schema = INVALID_SCHEMA;
	if (stmt->relation->schemaname) {
		info->schema = stmt->relation->schemaname;
	}
	info->table = stmt->relation->relname;
	info->on_conflict =
	    stmt->if_not_exists ? OnCreateConflict::IGNORE_ON_CONFLICT : OnCreateConflict::ERROR_ON_CONFLICT;
	info->temporary =
	    stmt->relation->relpersistence == guinsoodb_libpgquery::PGPostgresRelPersistence::PG_RELPERSISTENCE_TEMP;

	if (info->temporary && stmt->oncommit != guinsoodb_libpgquery::PGOnCommitAction::PG_ONCOMMIT_PRESERVE_ROWS &&
	    stmt->oncommit != guinsoodb_libpgquery::PGOnCommitAction::PG_ONCOMMIT_NOOP) {
		throw NotImplementedException("Only ON COMMIT PRESERVE ROWS is supported");
	}
	if (!stmt->tableElts) {
		throw ParserException("Table must have at least one column!");
	}

	for (auto c = stmt->tableElts->head; c != nullptr; c = lnext(c)) {
		auto node = reinterpret_cast<guinsoodb_libpgquery::PGNode *>(c->data.ptr_value);
		switch (node->type) {
		case guinsoodb_libpgquery::T_PGColumnDef: {
			auto cdef = (guinsoodb_libpgquery::PGColumnDef *)c->data.ptr_value;
			auto centry = TransformColumnDefinition(cdef);
			if (cdef->constraints) {
				for (auto constr = cdef->constraints->head; constr != nullptr; constr = constr->next) {
					auto constraint = TransformConstraint(constr, centry, info->columns.size());
					if (constraint) {
						info->constraints.push_back(move(constraint));
					}
				}
			}
			info->columns.push_back(move(centry));
			break;
		}
		case guinsoodb_libpgquery::T_PGConstraint: {
			info->constraints.push_back(TransformConstraint(c));
			break;
		}
		default:
			throw NotImplementedException("ColumnDef type not handled yet");
		}
	}
	result->info = move(info);
	return result;
}

} // namespace guinsoodb

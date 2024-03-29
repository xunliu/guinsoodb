#include "guinsoodb/parser/statement/alter_statement.hpp"
#include "guinsoodb/parser/transformer.hpp"
#include "guinsoodb/parser/expression/cast_expression.hpp"
#include "guinsoodb/parser/expression/columnref_expression.hpp"
#include "guinsoodb/parser/constraint.hpp"

namespace guinsoodb {

unique_ptr<AlterStatement> Transformer::TransformAlter(guinsoodb_libpgquery::PGNode *node) {
	auto stmt = reinterpret_cast<guinsoodb_libpgquery::PGAlterTableStmt *>(node);
	D_ASSERT(stmt);
	D_ASSERT(stmt->relation);

	auto result = make_unique<AlterStatement>();

	auto qname = TransformQualifiedName(stmt->relation);

	// first we check the type of ALTER
	for (auto c = stmt->cmds->head; c != nullptr; c = c->next) {
		auto command = reinterpret_cast<guinsoodb_libpgquery::PGAlterTableCmd *>(lfirst(c));
		// TODO: Include more options for command->subtype
		switch (command->subtype) {
		case guinsoodb_libpgquery::PG_AT_AddColumn: {
			auto cdef = (guinsoodb_libpgquery::PGColumnDef *)command->def;
			auto centry = TransformColumnDefinition(cdef);
			if (cdef->constraints) {
				for (auto constr = cdef->constraints->head; constr != nullptr; constr = constr->next) {
					auto constraint = TransformConstraint(constr, centry, 0);
					if (constraint) {
						throw ParserException("Adding columns with constraints not yet supported");
					}
				}
			}
			result->info = make_unique<AddColumnInfo>(qname.schema, qname.name, move(centry));
			break;
		}
		case guinsoodb_libpgquery::PG_AT_DropColumn: {
			result->info = make_unique<RemoveColumnInfo>(qname.schema, qname.name, command->name, command->missing_ok);
			break;
		}
		case guinsoodb_libpgquery::PG_AT_ColumnDefault: {
			auto expr = TransformExpression(command->def);
			result->info = make_unique<SetDefaultInfo>(qname.schema, qname.name, command->name, move(expr));
			break;
		}
		case guinsoodb_libpgquery::PG_AT_AlterColumnType: {
			auto cdef = (guinsoodb_libpgquery::PGColumnDef *)command->def;
			auto column_definition = TransformColumnDefinition(cdef);

			unique_ptr<ParsedExpression> expr;
			if (cdef->raw_default) {
				expr = TransformExpression(cdef->raw_default);
			} else {
				auto colref = make_unique<ColumnRefExpression>(command->name);
				expr = make_unique<CastExpression>(column_definition.type, move(colref));
			}
			result->info = make_unique<ChangeColumnTypeInfo>(qname.schema, qname.name, command->name,
			                                                 column_definition.type, move(expr));
			break;
		}
		case guinsoodb_libpgquery::PG_AT_DropConstraint:
		case guinsoodb_libpgquery::PG_AT_DropNotNull:
		default:
			throw NotImplementedException("ALTER TABLE option not supported yet!");
		}
	}

	return result;
}

} // namespace guinsoodb

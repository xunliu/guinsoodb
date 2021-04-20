#include "guinsoodb/parser/statement/insert_statement.hpp"
#include "guinsoodb/parser/tableref/expressionlistref.hpp"
#include "guinsoodb/parser/transformer.hpp"

namespace guinsoodb {

unique_ptr<TableRef> Transformer::TransformValuesList(guinsoodb_libpgquery::PGList *list) {
	auto result = make_unique<ExpressionListRef>();
	for (auto value_list = list->head; value_list != nullptr; value_list = value_list->next) {
		auto target = (guinsoodb_libpgquery::PGList *)(value_list->data.ptr_value);

		vector<unique_ptr<ParsedExpression>> insert_values;
		if (!TransformExpressionList(target, insert_values)) {
			throw ParserException("Could not parse expression list!");
		}
		if (!result->values.empty()) {
			if (result->values[0].size() != insert_values.size()) {
				throw ParserException("VALUES lists must all be the same length");
			}
		}
		result->values.push_back(move(insert_values));
	}
	result->alias = "valueslist";
	return move(result);
}

unique_ptr<InsertStatement> Transformer::TransformInsert(guinsoodb_libpgquery::PGNode *node) {
	auto stmt = reinterpret_cast<guinsoodb_libpgquery::PGInsertStmt *>(node);
	D_ASSERT(stmt);

	auto result = make_unique<InsertStatement>();

	// first check if there are any columns specified
	if (stmt->cols) {
		for (auto c = stmt->cols->head; c != nullptr; c = lnext(c)) {
			auto target = (guinsoodb_libpgquery::PGResTarget *)(c->data.ptr_value);
			result->columns.emplace_back(target->name);
		}
	}
	result->select_statement = TransformSelect(stmt->selectStmt, false);

	auto qname = TransformQualifiedName(stmt->relation);
	result->table = qname.name;
	result->schema = qname.schema;
	return result;
}

} // namespace guinsoodb

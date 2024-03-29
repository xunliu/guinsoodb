#include "guinsoodb/parser/statement/pragma_statement.hpp"
#include "guinsoodb/parser/transformer.hpp"
#include "guinsoodb/parser/expression/constant_expression.hpp"
#include "guinsoodb/parser/expression/comparison_expression.hpp"
#include "guinsoodb/execution/expression_executor.hpp"

namespace guinsoodb {

unique_ptr<PragmaStatement> Transformer::TransformPragma(guinsoodb_libpgquery::PGNode *node) {
	auto stmt = reinterpret_cast<guinsoodb_libpgquery::PGPragmaStmt *>(node);

	auto result = make_unique<PragmaStatement>();
	auto &info = *result->info;

	info.name = stmt->name;
	// parse the arguments, if any
	if (stmt->args) {
		for (auto cell = stmt->args->head; cell != nullptr; cell = cell->next) {
			auto node = reinterpret_cast<guinsoodb_libpgquery::PGNode *>(cell->data.ptr_value);
			auto expr = TransformExpression(node);

			if (expr->type == ExpressionType::COMPARE_EQUAL) {
				auto &comp = (ComparisonExpression &)*expr;
				info.named_parameters[comp.left->ToString()] = Value(comp.right->ToString());
			} else if (node->type == guinsoodb_libpgquery::T_PGAConst) {
				auto constant = TransformConstant((guinsoodb_libpgquery::PGAConst *)node);
				info.parameters.push_back(((ConstantExpression &)*constant).value);
			} else {
				info.parameters.emplace_back(expr->ToString());
			}
		}
	}
	// now parse the pragma type
	switch (stmt->kind) {
	case guinsoodb_libpgquery::PG_PRAGMA_TYPE_NOTHING:
		if (!info.parameters.empty() || !info.named_parameters.empty()) {
			throw ParserException("PRAGMA statement that is not a call or assignment cannot contain parameters");
		}
		break;
	case guinsoodb_libpgquery::PG_PRAGMA_TYPE_ASSIGNMENT:
		if (info.parameters.size() != 1) {
			throw ParserException("PRAGMA statement with assignment should contain exactly one parameter");
		}
		if (!info.named_parameters.empty()) {
			throw ParserException("PRAGMA statement with assignment cannot have named parameters");
		}
		break;
	case guinsoodb_libpgquery::PG_PRAGMA_TYPE_CALL:
		break;
	default:
		throw ParserException("Unknown pragma type");
	}

	return result;
}

} // namespace guinsoodb

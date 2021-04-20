#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/parser/expression/default_expression.hpp"
#include "guinsoodb/parser/transformer.hpp"

namespace guinsoodb {

unique_ptr<ParsedExpression> Transformer::TransformResTarget(guinsoodb_libpgquery::PGResTarget *root) {
	if (!root) {
		return nullptr;
	}
	auto expr = TransformExpression(root->val);
	if (!expr) {
		return nullptr;
	}
	if (root->name) {
		expr->alias = string(root->name);
	}
	return expr;
}

unique_ptr<ParsedExpression> Transformer::TransformNamedArg(guinsoodb_libpgquery::PGNamedArgExpr *root) {
	if (!root) {
		return nullptr;
	}
	auto expr = TransformExpression((guinsoodb_libpgquery::PGNode *)root->arg);
	if (root->name) {
		expr->alias = string(root->name);
	}
	return expr;
}

unique_ptr<ParsedExpression> Transformer::TransformExpression(guinsoodb_libpgquery::PGNode *node) {
	if (!node) {
		return nullptr;
	}

	switch (node->type) {
	case guinsoodb_libpgquery::T_PGColumnRef:
		return TransformColumnRef(reinterpret_cast<guinsoodb_libpgquery::PGColumnRef *>(node));
	case guinsoodb_libpgquery::T_PGAConst:
		return TransformConstant(reinterpret_cast<guinsoodb_libpgquery::PGAConst *>(node));
	case guinsoodb_libpgquery::T_PGAExpr:
		return TransformAExpr(reinterpret_cast<guinsoodb_libpgquery::PGAExpr *>(node));
	case guinsoodb_libpgquery::T_PGFuncCall:
		return TransformFuncCall(reinterpret_cast<guinsoodb_libpgquery::PGFuncCall *>(node));
	case guinsoodb_libpgquery::T_PGBoolExpr:
		return TransformBoolExpr(reinterpret_cast<guinsoodb_libpgquery::PGBoolExpr *>(node));
	case guinsoodb_libpgquery::T_PGTypeCast:
		return TransformTypeCast(reinterpret_cast<guinsoodb_libpgquery::PGTypeCast *>(node));
	case guinsoodb_libpgquery::T_PGCaseExpr:
		return TransformCase(reinterpret_cast<guinsoodb_libpgquery::PGCaseExpr *>(node));
	case guinsoodb_libpgquery::T_PGSubLink:
		return TransformSubquery(reinterpret_cast<guinsoodb_libpgquery::PGSubLink *>(node));
	case guinsoodb_libpgquery::T_PGCoalesceExpr:
		return TransformCoalesce(reinterpret_cast<guinsoodb_libpgquery::PGAExpr *>(node));
	case guinsoodb_libpgquery::T_PGNullTest:
		return TransformNullTest(reinterpret_cast<guinsoodb_libpgquery::PGNullTest *>(node));
	case guinsoodb_libpgquery::T_PGResTarget:
		return TransformResTarget(reinterpret_cast<guinsoodb_libpgquery::PGResTarget *>(node));
	case guinsoodb_libpgquery::T_PGParamRef:
		return TransformParamRef(reinterpret_cast<guinsoodb_libpgquery::PGParamRef *>(node));
	case guinsoodb_libpgquery::T_PGNamedArgExpr:
		return TransformNamedArg(reinterpret_cast<guinsoodb_libpgquery::PGNamedArgExpr *>(node));
	case guinsoodb_libpgquery::T_PGSQLValueFunction:
		return TransformSQLValueFunction(reinterpret_cast<guinsoodb_libpgquery::PGSQLValueFunction *>(node));
	case guinsoodb_libpgquery::T_PGSetToDefault:
		return make_unique<DefaultExpression>();
	case guinsoodb_libpgquery::T_PGCollateClause:
		return TransformCollateExpr(reinterpret_cast<guinsoodb_libpgquery::PGCollateClause *>(node));
	case guinsoodb_libpgquery::T_PGIntervalConstant:
		return TransformInterval(reinterpret_cast<guinsoodb_libpgquery::PGIntervalConstant *>(node));
	case guinsoodb_libpgquery::T_PGLambdaFunction:
		return TransformLambda(reinterpret_cast<guinsoodb_libpgquery::PGLambdaFunction *>(node));
	case guinsoodb_libpgquery::T_PGAIndirection:
		return TransformArrayAccess(reinterpret_cast<guinsoodb_libpgquery::PGAIndirection *>(node));
	case guinsoodb_libpgquery::T_PGPositionalReference:
		return TransformPositionalReference(reinterpret_cast<guinsoodb_libpgquery::PGPositionalReference *>(node));
	default:
		throw NotImplementedException("Expr of type %d not implemented\n", (int)node->type);
	}
}

bool Transformer::TransformExpressionList(guinsoodb_libpgquery::PGList *list,
                                          vector<unique_ptr<ParsedExpression>> &result) {
	if (!list) {
		return false;
	}
	for (auto node = list->head; node != nullptr; node = node->next) {
		auto target = reinterpret_cast<guinsoodb_libpgquery::PGNode *>(node->data.ptr_value);
		if (!target) {
			return false;
		}
		auto expr = TransformExpression(target);
		if (!expr) {
			return false;
		}
		result.push_back(move(expr));
	}
	return true;
}

} // namespace guinsoodb

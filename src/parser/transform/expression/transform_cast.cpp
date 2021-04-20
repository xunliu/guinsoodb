#include "guinsoodb/common/limits.hpp"
#include "guinsoodb/parser/expression/cast_expression.hpp"
#include "guinsoodb/parser/expression/constant_expression.hpp"
#include "guinsoodb/parser/transformer.hpp"
#include "guinsoodb/common/operator/cast_operators.hpp"

namespace guinsoodb {

unique_ptr<ParsedExpression> Transformer::TransformTypeCast(guinsoodb_libpgquery::PGTypeCast *root) {
	if (!root) {
		return nullptr;
	}
	// get the type to cast to
	auto type_name = root->typeName;
	LogicalType target_type = TransformTypeName(type_name);

	// check for a constant BLOB value, then return ConstantExpression with BLOB
	if (target_type == LogicalType::BLOB && root->arg->type == guinsoodb_libpgquery::T_PGAConst) {
		auto c = reinterpret_cast<guinsoodb_libpgquery::PGAConst *>(root->arg);
		if (c->val.type == guinsoodb_libpgquery::T_PGString) {
			return make_unique<ConstantExpression>(Value::BLOB(string(c->val.val.str)));
		}
	}
	// transform the expression node
	auto expression = TransformExpression(root->arg);
	bool try_cast = root->tryCast;

	// now create a cast operation
	return make_unique<CastExpression>(target_type, move(expression), try_cast);
}

} // namespace guinsoodb

#include "guinsoodb/main/relation/table_function_relation.hpp"
#include "guinsoodb/parser/tableref/basetableref.hpp"
#include "guinsoodb/parser/query_node/select_node.hpp"
#include "guinsoodb/parser/expression/star_expression.hpp"
#include "guinsoodb/parser/tableref/table_function_ref.hpp"
#include "guinsoodb/parser/expression/constant_expression.hpp"
#include "guinsoodb/parser/expression/function_expression.hpp"
#include "guinsoodb/parser/expression/subquery_expression.hpp"
#include "guinsoodb/main/client_context.hpp"

namespace guinsoodb {

TableFunctionRelation::TableFunctionRelation(ClientContext &context, string name_p, vector<Value> parameters_p,
                                             shared_ptr<Relation> input_relation_p)
    : Relation(context, RelationType::TABLE_FUNCTION_RELATION), name(move(name_p)), parameters(move(parameters_p)),
      input_relation(move(input_relation_p)) {
	context.TryBindRelation(*this, this->columns);
}

unique_ptr<QueryNode> TableFunctionRelation::GetQueryNode() {
	auto result = make_unique<SelectNode>();
	result->select_list.push_back(make_unique<StarExpression>());
	result->from_table = GetTableRef();
	return move(result);
}

unique_ptr<TableRef> TableFunctionRelation::GetTableRef() {
	vector<unique_ptr<ParsedExpression>> children;
	if (input_relation) { // input relation becomes first parameter if present, always
		auto subquery = make_unique<SubqueryExpression>();
		subquery->subquery = make_unique<SelectStatement>();
		subquery->subquery->node = input_relation->GetQueryNode();
		subquery->subquery_type = SubqueryType::SCALAR;
		children.push_back(move(subquery));
	}
	for (auto &parameter : parameters) {
		children.push_back(make_unique<ConstantExpression>(parameter));
	}

	auto table_function = make_unique<TableFunctionRef>();
	auto function = make_unique<FunctionExpression>(name, children);
	table_function->function = move(function);
	return move(table_function);
}

string TableFunctionRelation::GetAlias() {
	return name;
}

const vector<ColumnDefinition> &TableFunctionRelation::Columns() {
	return columns;
}

string TableFunctionRelation::ToString(idx_t depth) {
	string function_call = name + "(";
	for (idx_t i = 0; i < parameters.size(); i++) {
		if (i > 0) {
			function_call += ", ";
		}
		function_call += parameters[i].ToString();
	}
	function_call += ")";
	return RenderWhitespace(depth) + function_call;
}

} // namespace guinsoodb

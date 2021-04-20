#include "guinsoodb/main/relation/explain_relation.hpp"
#include "guinsoodb/parser/statement/explain_statement.hpp"
#include "guinsoodb/parser/statement/select_statement.hpp"
#include "guinsoodb/parser/parsed_data/create_view_info.hpp"
#include "guinsoodb/planner/binder.hpp"
#include "guinsoodb/main/client_context.hpp"

namespace guinsoodb {

ExplainRelation::ExplainRelation(shared_ptr<Relation> child_p)
    : Relation(child_p->context, RelationType::EXPLAIN_RELATION), child(move(child_p)) {
	context.TryBindRelation(*this, this->columns);
}

unique_ptr<QueryNode> ExplainRelation::GetQueryNode() {
	throw InternalException("Cannot create a query node from a ExplainRelation!");
}

BoundStatement ExplainRelation::Bind(Binder &binder) {
	auto select = make_unique<SelectStatement>();
	select->node = child->GetQueryNode();
	ExplainStatement explain(move(select));
	return binder.Bind((SQLStatement &)explain);
}

const vector<ColumnDefinition> &ExplainRelation::Columns() {
	return columns;
}

string ExplainRelation::ToString(idx_t depth) {
	string str = RenderWhitespace(depth) + "Explain\n";
	return str + child->ToString(depth + 1);
}

} // namespace guinsoodb

#include "guinsoodb/main/relation/subquery_relation.hpp"
#include "guinsoodb/main/client_context.hpp"
#include "guinsoodb/parser/query_node.hpp"

namespace guinsoodb {

SubqueryRelation::SubqueryRelation(shared_ptr<Relation> child_p, string alias_p)
    : Relation(child_p->context, RelationType::SUBQUERY_RELATION), child(move(child_p)), alias(move(alias_p)) {
	vector<ColumnDefinition> dummy_columns;
	context.TryBindRelation(*this, dummy_columns);
}

unique_ptr<QueryNode> SubqueryRelation::GetQueryNode() {
	return child->GetQueryNode();
}

string SubqueryRelation::GetAlias() {
	return alias;
}

const vector<ColumnDefinition> &SubqueryRelation::Columns() {
	return child->Columns();
}

string SubqueryRelation::ToString(idx_t depth) {
	return child->ToString(depth);
}

} // namespace guinsoodb

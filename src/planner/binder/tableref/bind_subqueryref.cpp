#include "guinsoodb/parser/tableref/subqueryref.hpp"
#include "guinsoodb/planner/binder.hpp"
#include "guinsoodb/planner/tableref/bound_subqueryref.hpp"

namespace guinsoodb {

unique_ptr<BoundTableRef> Binder::Bind(SubqueryRef &ref, CommonTableExpressionInfo *cte) {
	auto binder = Binder::CreateBinder(context, this);
	if (cte) {
		binder->bound_ctes.insert(cte);
	}
	binder->alias = ref.alias;
	auto subquery = binder->BindNode(*ref.subquery->node);
	idx_t bind_index = subquery->GetRootIndex();
	auto result = make_unique<BoundSubqueryRef>(move(binder), move(subquery));

	bind_context.AddSubquery(bind_index, ref.alias, ref, *result->subquery);
	MoveCorrelatedExpressions(*result->binder);
	return move(result);
}

} // namespace guinsoodb

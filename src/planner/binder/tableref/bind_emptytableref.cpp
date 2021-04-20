#include "guinsoodb/parser/tableref/emptytableref.hpp"
#include "guinsoodb/planner/binder.hpp"
#include "guinsoodb/planner/tableref/bound_dummytableref.hpp"

namespace guinsoodb {

unique_ptr<BoundTableRef> Binder::Bind(EmptyTableRef &ref) {
	return make_unique<BoundEmptyTableRef>(GenerateTableIndex());
}

} // namespace guinsoodb

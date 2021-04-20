#include "guinsoodb/planner/operator/logical_empty_result.hpp"

namespace guinsoodb {

LogicalEmptyResult::LogicalEmptyResult(unique_ptr<LogicalOperator> op)
    : LogicalOperator(LogicalOperatorType::LOGICAL_EMPTY_RESULT) {

	this->bindings = op->GetColumnBindings();

	op->ResolveOperatorTypes();
	this->return_types = op->types;
}

} // namespace guinsoodb

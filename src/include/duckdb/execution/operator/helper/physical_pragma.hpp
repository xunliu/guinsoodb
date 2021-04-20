//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/execution/operator/helper/physical_pragma.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/execution/physical_operator.hpp"
#include "guinsoodb/parser/parsed_data/pragma_info.hpp"
#include "guinsoodb/function/pragma_function.hpp"

namespace guinsoodb {

//! PhysicalPragma represents the PRAGMA operator
class PhysicalPragma : public PhysicalOperator {
public:
	PhysicalPragma(PragmaFunction function_p, PragmaInfo info_p, idx_t estimated_cardinality)
	    : PhysicalOperator(PhysicalOperatorType::PRAGMA, {LogicalType::BOOLEAN}, estimated_cardinality),
	      function(move(function_p)), info(move(info_p)) {
	}

	//! The pragma function to call
	PragmaFunction function;
	//! The context of the call
	PragmaInfo info;

public:
	void GetChunkInternal(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state) override;
};

} // namespace guinsoodb

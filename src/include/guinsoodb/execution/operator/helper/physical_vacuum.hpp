//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/execution/operator/helper/physical_vacuum.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/execution/physical_operator.hpp"
#include "guinsoodb/parser/parsed_data/vacuum_info.hpp"

namespace guinsoodb {

//! PhysicalVacuum represents a VACUUM operation (e.g. VACUUM or ANALYZE)
class PhysicalVacuum : public PhysicalOperator {
public:
	explicit PhysicalVacuum(unique_ptr<VacuumInfo> info, idx_t estimated_cardinality)
	    : PhysicalOperator(PhysicalOperatorType::VACUUM, {LogicalType::BOOLEAN}, estimated_cardinality),
	      info(move(info)) {
	}

	unique_ptr<VacuumInfo> info;

public:
	void GetChunkInternal(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state) override;
};

} // namespace guinsoodb

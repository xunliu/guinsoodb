//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/execution/operator/helper/physical_transaction.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/execution/physical_operator.hpp"
#include "guinsoodb/parser/parsed_data/transaction_info.hpp"

namespace guinsoodb {

//! PhysicalTransaction represents a transaction operator (e.g. BEGIN or COMMIT)
class PhysicalTransaction : public PhysicalOperator {
public:
	explicit PhysicalTransaction(unique_ptr<TransactionInfo> info, idx_t estimated_cardinality)
	    : PhysicalOperator(PhysicalOperatorType::TRANSACTION, {LogicalType::BOOLEAN}, estimated_cardinality),
	      info(move(info)) {
	}

	unique_ptr<TransactionInfo> info;

public:
	void GetChunkInternal(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state) override;
};

} // namespace guinsoodb

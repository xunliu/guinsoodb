//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/parsed_data/transaction_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_data/parse_info.hpp"

namespace guinsoodb {

enum class TransactionType : uint8_t { INVALID, BEGIN_TRANSACTION, COMMIT, ROLLBACK };

struct TransactionInfo : public ParseInfo {
	explicit TransactionInfo(TransactionType type) : type(type) {
	}

	//! The type of transaction statement
	TransactionType type;
};

} // namespace guinsoodb

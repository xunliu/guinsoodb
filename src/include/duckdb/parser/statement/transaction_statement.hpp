//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/statement/transaction_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/sql_statement.hpp"
#include "guinsoodb/parser/parsed_data/transaction_info.hpp"

namespace guinsoodb {

class TransactionStatement : public SQLStatement {
public:
	explicit TransactionStatement(TransactionType type);

	unique_ptr<TransactionInfo> info;

public:
	unique_ptr<SQLStatement> Copy() const override;
};
} // namespace guinsoodb

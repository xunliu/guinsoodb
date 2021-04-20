//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/sql_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/common.hpp"
#include "guinsoodb/common/enums/statement_type.hpp"
#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/common/printer.hpp"

namespace guinsoodb {
//! SQLStatement is the base class of any type of SQL statement.
class SQLStatement {
public:
	explicit SQLStatement(StatementType type) : type(type) {};
	virtual ~SQLStatement() {
	}

	//! The statement type
	StatementType type;
	//! The statement location within the query string
	idx_t stmt_location;
	//! The statement length within the query string
	idx_t stmt_length;
	//! The number of prepared statement parameters (if any)
	idx_t n_param;
	//! The query text that corresponds to this SQL statement
	string query;

public:
	//! Create a copy of this SelectStatement
	virtual unique_ptr<SQLStatement> Copy() const = 0;
};
} // namespace guinsoodb

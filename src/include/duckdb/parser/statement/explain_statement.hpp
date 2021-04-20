//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/statement/explain_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_expression.hpp"
#include "guinsoodb/parser/sql_statement.hpp"

namespace guinsoodb {

class ExplainStatement : public SQLStatement {
public:
	explicit ExplainStatement(unique_ptr<SQLStatement> stmt);

	unique_ptr<SQLStatement> stmt;

public:
	unique_ptr<SQLStatement> Copy() const override;
};

} // namespace guinsoodb

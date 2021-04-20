//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/statement/delete_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_expression.hpp"
#include "guinsoodb/parser/sql_statement.hpp"
#include "guinsoodb/parser/tableref.hpp"

namespace guinsoodb {

class DeleteStatement : public SQLStatement {
public:
	DeleteStatement();

	unique_ptr<ParsedExpression> condition;
	unique_ptr<TableRef> table;

public:
	unique_ptr<SQLStatement> Copy() const override;
};

} // namespace guinsoodb

//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/statement/update_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_expression.hpp"
#include "guinsoodb/parser/sql_statement.hpp"
#include "guinsoodb/parser/tableref.hpp"
#include "guinsoodb/common/vector.hpp"

namespace guinsoodb {

class UpdateStatement : public SQLStatement {
public:
	UpdateStatement();

	unique_ptr<ParsedExpression> condition;
	unique_ptr<TableRef> table;
	unique_ptr<TableRef> from_table;
	vector<string> columns;
	vector<unique_ptr<ParsedExpression>> expressions;

public:
	unique_ptr<SQLStatement> Copy() const override;
};

} // namespace guinsoodb

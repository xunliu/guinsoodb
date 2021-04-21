//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/statement/execute_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_expression.hpp"
#include "guinsoodb/parser/sql_statement.hpp"
#include "guinsoodb/common/vector.hpp"

namespace guinsoodb {

class ExecuteStatement : public SQLStatement {
public:
	ExecuteStatement();

	string name;
	vector<unique_ptr<ParsedExpression>> values;

public:
	unique_ptr<SQLStatement> Copy() const override;
};
} // namespace guinsoodb

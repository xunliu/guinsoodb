//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/statement/call_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_expression.hpp"
#include "guinsoodb/parser/sql_statement.hpp"
#include "guinsoodb/common/vector.hpp"

namespace guinsoodb {

class CallStatement : public SQLStatement {
public:
	CallStatement();

	unique_ptr<ParsedExpression> function;

public:
	unique_ptr<SQLStatement> Copy() const override;
};
} // namespace guinsoodb

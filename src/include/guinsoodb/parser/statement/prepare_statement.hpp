//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/statement/prepare_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_expression.hpp"
#include "guinsoodb/parser/sql_statement.hpp"

namespace guinsoodb {

class PrepareStatement : public SQLStatement {
public:
	PrepareStatement();

	unique_ptr<SQLStatement> statement;
	string name;

public:
	unique_ptr<SQLStatement> Copy() const override;
};
} // namespace guinsoodb

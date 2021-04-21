//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/statement/set_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/sql_statement.hpp"
#include "guinsoodb/common/types/value.hpp"

namespace guinsoodb {

class SetStatement : public SQLStatement {
public:
	SetStatement(std::string name_p, Value value_p);

public:
	unique_ptr<SQLStatement> Copy() const override;

	std::string name;
	Value value;
};
} // namespace guinsoodb

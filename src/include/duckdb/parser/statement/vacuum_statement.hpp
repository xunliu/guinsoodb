//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/statement/vacuum_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_expression.hpp"
#include "guinsoodb/parser/sql_statement.hpp"
#include "guinsoodb/parser/parsed_data/vacuum_info.hpp"

namespace guinsoodb {

class VacuumStatement : public SQLStatement {
public:
	VacuumStatement();

	unique_ptr<VacuumInfo> info;

public:
	unique_ptr<SQLStatement> Copy() const override;
};

} // namespace guinsoodb

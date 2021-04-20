//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/statement/drop_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_data/drop_info.hpp"
#include "guinsoodb/parser/sql_statement.hpp"

namespace guinsoodb {

class DropStatement : public SQLStatement {
public:
	DropStatement();

	unique_ptr<DropInfo> info;

public:
	unique_ptr<SQLStatement> Copy() const override;
};

} // namespace guinsoodb

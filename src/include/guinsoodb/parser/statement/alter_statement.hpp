//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/statement/alter_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/column_definition.hpp"
#include "guinsoodb/parser/parsed_data/alter_table_info.hpp"
#include "guinsoodb/parser/sql_statement.hpp"

namespace guinsoodb {

class AlterStatement : public SQLStatement {
public:
	AlterStatement();
	explicit AlterStatement(unique_ptr<AlterInfo> info);

	unique_ptr<AlterInfo> info;

public:
	unique_ptr<SQLStatement> Copy() const override;
};

} // namespace guinsoodb

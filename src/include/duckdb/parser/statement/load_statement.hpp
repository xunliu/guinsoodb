//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/statement/set_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/sql_statement.hpp"
#include "guinsoodb/parser/parsed_data/load_info.hpp"

namespace guinsoodb {

class LoadStatement : public SQLStatement {
public:
	LoadStatement();

public:
	unique_ptr<SQLStatement> Copy() const override;

	unique_ptr<LoadInfo> info;
};
} // namespace guinsoodb

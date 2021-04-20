//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/statement/show_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/sql_statement.hpp"
#include "guinsoodb/parser/parsed_data/show_select_info.hpp"

namespace guinsoodb {

class ShowStatement : public SQLStatement {
public:
	ShowStatement();

	unique_ptr<ShowSelectInfo> info;

public:
	unique_ptr<SQLStatement> Copy() const override;
};

} // namespace guinsoodb

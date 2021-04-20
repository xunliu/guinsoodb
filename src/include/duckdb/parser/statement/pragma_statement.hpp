//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/statement/pragma_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/sql_statement.hpp"
#include "guinsoodb/parser/parsed_data/pragma_info.hpp"
#include "guinsoodb/parser/parsed_expression.hpp"

namespace guinsoodb {

class PragmaStatement : public SQLStatement {
public:
	PragmaStatement();

	unique_ptr<PragmaInfo> info;

public:
	unique_ptr<SQLStatement> Copy() const override;
};

} // namespace guinsoodb

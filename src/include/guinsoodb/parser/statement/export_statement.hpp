//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/statement/export_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_expression.hpp"
#include "guinsoodb/parser/sql_statement.hpp"
#include "guinsoodb/parser/parsed_data/copy_info.hpp"

namespace guinsoodb {

class ExportStatement : public SQLStatement {
public:
	explicit ExportStatement(unique_ptr<CopyInfo> info);

	unique_ptr<CopyInfo> info;

public:
	unique_ptr<SQLStatement> Copy() const override;
};

} // namespace guinsoodb

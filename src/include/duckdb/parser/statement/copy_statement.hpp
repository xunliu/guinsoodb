//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/statement/copy_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_data/copy_info.hpp"
#include "guinsoodb/parser/query_node.hpp"
#include "guinsoodb/parser/sql_statement.hpp"

namespace guinsoodb {

class CopyStatement : public SQLStatement {
public:
	CopyStatement();

	unique_ptr<CopyInfo> info;
	// The SQL statement used instead of a table when copying data out to a file
	unique_ptr<QueryNode> select_statement;

public:
	unique_ptr<SQLStatement> Copy() const override;
};
} // namespace guinsoodb

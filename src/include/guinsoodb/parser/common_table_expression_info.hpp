//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/common_table_expression_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/statement/select_statement.hpp"

namespace guinsoodb {

class SelectStatement;

struct CommonTableExpressionInfo {
	vector<string> aliases;
	unique_ptr<SelectStatement> query;
};

} // namespace guinsoodb

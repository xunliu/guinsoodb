#include "guinsoodb/parser/statement/export_statement.hpp"

namespace guinsoodb {

ExportStatement::ExportStatement(unique_ptr<CopyInfo> info)
    : SQLStatement(StatementType::EXPORT_STATEMENT), info(move(info)) {
}

unique_ptr<SQLStatement> ExportStatement::Copy() const {
	return make_unique_base<SQLStatement, ExportStatement>(info->Copy());
}

} // namespace guinsoodb

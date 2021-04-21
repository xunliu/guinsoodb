//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/pragma_handler.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/common.hpp"
#include "guinsoodb/common/vector.hpp"
#include "guinsoodb/parser/statement/pragma_statement.hpp"

namespace guinsoodb {
class ClientContext;
class ClientContextLock;
class SQLStatement;
struct PragmaInfo;

//! Pragma handler is responsible for converting certain pragma statements into new queries
class PragmaHandler {
public:
	explicit PragmaHandler(ClientContext &context);

	void HandlePragmaStatements(ClientContextLock &lock, vector<unique_ptr<SQLStatement>> &statements);

private:
	ClientContext &context;

private:
	//! Handles a pragma statement, (potentially) returning a new statement to replace the current one
	string HandlePragma(SQLStatement *statement);

	void HandlePragmaStatementsInternal(vector<unique_ptr<SQLStatement>> &statements);
};
} // namespace guinsoodb

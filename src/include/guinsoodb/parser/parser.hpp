//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/parser.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/sql_statement.hpp"
#include "guinsoodb/parser/parsed_expression.hpp"
#include "guinsoodb/parser/query_node.hpp"
#include "guinsoodb/parser/column_definition.hpp"
#include "guinsoodb/parser/simplified_token.hpp"

namespace guinsoodb_libpgquery {
struct PGNode;
struct PGList;
} // namespace guinsoodb_libpgquery

namespace guinsoodb {

//! The parser is responsible for parsing the query and converting it into a set
//! of parsed statements. The parsed statements can then be converted into a
//! plan and executed.
class Parser {
public:
	Parser();

	//! Attempts to parse a query into a series of SQL statements. Returns
	//! whether or not the parsing was successful. If the parsing was
	//! successful, the parsed statements will be stored in the statements
	//! variable.
	void ParseQuery(const string &query);

	//! Tokenize a query, returning the raw tokens together with their locations
	static vector<SimplifiedToken> Tokenize(const string &query);

	//! Returns true if the given text matches a keyword of the parser
	static bool IsKeyword(const string &text);

	//! Parses a list of expressions (i.e. the list found in a SELECT clause)
	static vector<unique_ptr<ParsedExpression>> ParseExpressionList(const string &select_list);
	//! Parses a list as found in an ORDER BY expression (i.e. including optional ASCENDING/DESCENDING modifiers)
	static vector<OrderByNode> ParseOrderList(const string &select_list);
	//! Parses an update list (i.e. the list found in the SET clause of an UPDATE statement)
	static void ParseUpdateList(const string &update_list, vector<string> &update_columns,
	                            vector<unique_ptr<ParsedExpression>> &expressions);
	//! Parses a VALUES list (i.e. the list of expressions after a VALUES clause)
	static vector<vector<unique_ptr<ParsedExpression>>> ParseValuesList(const string &value_list);
	//! Parses a column list (i.e. as found in a CREATE TABLE statement)
	static vector<ColumnDefinition> ParseColumnList(const string &column_list);

	//! The parsed SQL statements from an invocation to ParseQuery.
	vector<unique_ptr<SQLStatement>> statements;

private:
	//! Transform a Postgres parse tree into a set of SQL Statements
	bool TransformList(guinsoodb_libpgquery::PGList *tree);
	//! Transform a single Postgres parse node into a SQL Statement.
	unique_ptr<SQLStatement> TransformNode(guinsoodb_libpgquery::PGNode *stmt);
};
} // namespace guinsoodb

//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// postgres_parser.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <string>
#include <vector>
#include "nodes/pg_list.hpp"
#include "pg_simplified_token.hpp"

namespace guinsoodb {
class PostgresParser {
public:
	PostgresParser();
	~PostgresParser();

	bool success;
	guinsoodb_libpgquery::PGList *parse_tree;
	std::string error_message;
	int error_location;
public:
	void Parse(const std::string &query);
	static std::vector<guinsoodb_libpgquery::PGSimplifiedToken> Tokenize(const std::string &query);

	static bool IsKeyword(const std::string &text);
};
}

#include "postgres_parser.hpp"

#include "pg_functions.hpp"
#include "parser/parser.hpp"
#include "common/keywords.hpp"

using namespace std;

namespace guinsoodb {

PostgresParser::PostgresParser() : success(false), parse_tree(nullptr), error_message(""), error_location(0) {}

void PostgresParser::Parse(const string &query) {
	guinsoodb_libpgquery::pg_parser_init();
    guinsoodb_libpgquery::parse_result res;
	pg_parser_parse(query.c_str(), &res);
	success = res.success;

	if (success) {
		parse_tree = res.parse_tree;
	} else {
		error_message = string(res.error_message);
		error_location = res.error_location;
	}
}

vector<guinsoodb_libpgquery::PGSimplifiedToken> PostgresParser::Tokenize(const std::string &query) {
	guinsoodb_libpgquery::pg_parser_init();
	auto tokens = guinsoodb_libpgquery::tokenize(query.c_str());
	guinsoodb_libpgquery::pg_parser_cleanup();
	return tokens;
}

PostgresParser::~PostgresParser()  {
    guinsoodb_libpgquery::pg_parser_cleanup();
}

bool PostgresParser::IsKeyword(const std::string &text) {
	return guinsoodb_libpgquery::is_keyword(text.c_str());
}

}

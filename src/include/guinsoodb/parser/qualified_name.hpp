//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/qualified_name.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/string.hpp"
#include "guinsoodb/common/exception.hpp"

namespace guinsoodb {

struct QualifiedName {
	string schema;
	string name;

	//! Parse the (optional) schema and a name from a string in the format of e.g. "schema"."table"; if there is no dot
	//! the schema will be set to INVALID_SCHEMA
	static QualifiedName Parse(string input) {
		string schema;
		string name;
		idx_t idx = 0;
		vector<string> entries;
		string entry;
	normal:
		// quote
		for (; idx < input.size(); idx++) {
			if (input[idx] == '"') {
				idx++;
				goto quoted;
			} else if (input[idx] == '.') {
				goto separator;
			}
			entry += input[idx];
		}
		goto end;
	separator:
		entries.push_back(entry);
		entry = "";
		idx++;
		goto normal;
	quoted:
		// look for another quote
		for (; idx < input.size(); idx++) {
			if (input[idx] == '"') {
				// unquote
				idx++;
				goto normal;
			}
			entry += input[idx];
		}
		throw ParserException("Unterminated quote in qualified name!");
	end:
		if (entries.size() == 0) {
			schema = INVALID_SCHEMA;
			name = entry;
		} else if (entries.size() == 1) {
			schema = entries[0];
			name = entry;
		} else {
			throw ParserException("Expected schema.entry or entry: too many entries found");
		}
		return QualifiedName {schema, name};
	}
};

struct QualifiedColumnName {
	QualifiedColumnName() {
	}
	QualifiedColumnName(string table_p, string column_p) : table(move(table_p)), column(move(column_p)) {
	}

	string schema;
	string table;
	string column;
};

} // namespace guinsoodb

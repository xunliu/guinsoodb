//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/query_error_context.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/common.hpp"
#include "guinsoodb/common/vector.hpp"
#include "guinsoodb/common/exception_format_value.hpp"

namespace guinsoodb {
class SQLStatement;

class QueryErrorContext {
public:
	explicit QueryErrorContext(SQLStatement *statement_ = nullptr, idx_t query_location_ = INVALID_INDEX)
	    : statement(statement_), query_location(query_location_) {
	}

	//! The query statement
	SQLStatement *statement;
	//! The location in which the error should be thrown
	idx_t query_location;

public:
	static string Format(const string &query, const string &error_message, int error_location);

	string FormatErrorRecursive(const string &msg, vector<ExceptionFormatValue> &values);
	template <class T, typename... Args>
	string FormatErrorRecursive(const string &msg, vector<ExceptionFormatValue> &values, T param, Args... params) {
		values.push_back(ExceptionFormatValue::CreateFormatValue<T>(param));
		return FormatErrorRecursive(msg, values, params...);
	}

	template <typename... Args>
	string FormatError(const string &msg, Args... params) {
		vector<ExceptionFormatValue> values;
		return FormatErrorRecursive(msg, values, params...);
	}
};

} // namespace guinsoodb

//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/function/pragma_function.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/function/function.hpp"
#include "guinsoodb/parser/parsed_data/pragma_info.hpp"
#include "guinsoodb/common/unordered_map.hpp"

namespace guinsoodb {
class ClientContext;

//! Return a substitute query to execute instead of this pragma statement
typedef string (*pragma_query_t)(ClientContext &context, const FunctionParameters &parameters);
//! Execute the main pragma function
typedef void (*pragma_function_t)(ClientContext &context, const FunctionParameters &parameters);

//! Pragma functions are invoked by calling PRAGMA x
//! Pragma functions come in three types:
//! * Call: function call, e.g. PRAGMA table_info('tbl')
//!   -> call statements can take multiple parameters
//! * Statement: statement without parameters, e.g. PRAGMA show_tables
//!   -> this is similar to a call pragma but without parameters
//! * Assignment: value assignment, e.g. PRAGMA memory_limit='8GB'
//!   -> assignments take a single parameter
//!   -> assignments can also be called through SET memory_limit='8GB'
//! Pragma functions can either return a new query to execute (pragma_query_t)
//! or they can
class PragmaFunction : public SimpleNamedParameterFunction {
public:
	// Call
	static PragmaFunction PragmaCall(const string &name, pragma_query_t query, vector<LogicalType> arguments,
	                                 LogicalType varargs = LogicalType::INVALID);
	static PragmaFunction PragmaCall(const string &name, pragma_function_t function, vector<LogicalType> arguments,
	                                 LogicalType varargs = LogicalType::INVALID);
	// Statement
	static PragmaFunction PragmaStatement(const string &name, pragma_query_t query);
	static PragmaFunction PragmaStatement(const string &name, pragma_function_t function);
	// Assignment
	static PragmaFunction PragmaAssignment(const string &name, pragma_query_t query, LogicalType type);
	static PragmaFunction PragmaAssignment(const string &name, pragma_function_t function, LogicalType type);

	string ToString() override;

public:
	PragmaType type;

	pragma_query_t query;
	pragma_function_t function;
	unordered_map<string, LogicalType> named_parameters;

private:
	PragmaFunction(string name, PragmaType pragma_type, pragma_query_t query, pragma_function_t function,
	               vector<LogicalType> arguments, LogicalType varargs);
};

} // namespace guinsoodb

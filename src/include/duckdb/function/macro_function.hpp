//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/function/macro_function.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/function/function.hpp"
#include "guinsoodb/main/client_context.hpp"
#include "guinsoodb/planner/binder.hpp"
#include "guinsoodb/planner/expression_binder.hpp"

namespace guinsoodb {

class MacroCatalogEntry;

class MacroFunction {
public:
	explicit MacroFunction(unique_ptr<ParsedExpression> expression);

	//! Check whether the supplied arguments are valid
	static string ValidateArguments(MacroCatalogEntry &macro_func, FunctionExpression &function_expr,
	                                vector<unique_ptr<ParsedExpression>> &positionals,
	                                unordered_map<string, unique_ptr<ParsedExpression>> &defaults);
	//! The macro expression
	unique_ptr<ParsedExpression> expression;
	//! The positional parameters
	vector<unique_ptr<ParsedExpression>> parameters;
	//! The default parameters and their associated values
	unordered_map<string, unique_ptr<ParsedExpression>> default_parameters;

public:
	unique_ptr<MacroFunction> Copy();
};

} // namespace guinsoodb

//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/table_binding.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/common.hpp"
#include "guinsoodb/common/unordered_map.hpp"
#include "guinsoodb/parser/column_definition.hpp"
#include "guinsoodb/parser/parsed_expression.hpp"
#include "guinsoodb/planner/expression_binder.hpp"

namespace guinsoodb {
class BindContext;
class BoundQueryNode;
class ColumnRefExpression;
class SubqueryRef;
class LogicalGet;
class TableCatalogEntry;
class TableFunctionCatalogEntry;
class BoundTableFunction;

//! A Binding represents a binding to a table, table-producing function or subquery with a specified table index.
struct Binding {
	Binding(const string &alias, vector<LogicalType> types, vector<string> names, idx_t index);
	virtual ~Binding() = default;

	//! The alias of the binding
	string alias;
	//! The table index of the binding
	idx_t index;
	vector<LogicalType> types;
	//! Column names of the subquery
	vector<string> names;
	//! Name -> index for the names
	unordered_map<string, column_t> name_map;

public:
	bool HasMatchingBinding(const string &column_name);
	virtual BindResult Bind(ColumnRefExpression &colref, idx_t depth);
};

//! TableBinding is exactly like the Binding, except it keeps track of which columns were bound in the linked LogicalGet
//! node for projection pushdown purposes.
struct TableBinding : public Binding {
	TableBinding(const string &alias, vector<LogicalType> types, vector<string> names, LogicalGet &get, idx_t index,
	             bool add_row_id = false);

	//! the underlying LogicalGet
	LogicalGet &get;

public:
	BindResult Bind(ColumnRefExpression &colref, idx_t depth) override;
};

//! MacroBinding is like the Binding, except the alias and index are set by default. Used for binding Macro
//! Params/Arguments.
struct MacroBinding : public Binding {
	MacroBinding(vector<LogicalType> types_p, vector<string> names_p, string macro_name);

	//! Arguments
	vector<unique_ptr<ParsedExpression>> arguments;
	//! The name of the macro
	string macro_name;

public:
	BindResult Bind(ColumnRefExpression &colref, idx_t depth) override;

	//! Given the parameter colref, returns a copy of the argument that was supplied for this parameter
	unique_ptr<ParsedExpression> ParamToArg(ColumnRefExpression &colref);
};

} // namespace guinsoodb

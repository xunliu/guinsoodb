//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/planner/bind_context.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/catalog/catalog.hpp"
#include "guinsoodb/catalog/catalog_entry/table_catalog_entry.hpp"
#include "guinsoodb/catalog/catalog_entry/table_function_catalog_entry.hpp"
#include "guinsoodb/common/unordered_map.hpp"
#include "guinsoodb/common/unordered_set.hpp"
#include "guinsoodb/parser/expression/columnref_expression.hpp"
#include "guinsoodb/parser/parsed_expression.hpp"
#include "guinsoodb/parser/qualified_name_set.hpp"
#include "guinsoodb/planner/expression.hpp"
#include "guinsoodb/planner/expression_binder.hpp"
#include "guinsoodb/planner/table_binding.hpp"

namespace guinsoodb {
class Binder;
class LogicalGet;
class BoundQueryNode;

struct UsingColumnSet {
	string primary_binding;
	unordered_set<string> bindings;
};

//! The BindContext object keeps track of all the tables and columns that are
//! encountered during the binding process.
class BindContext {
public:
	//! Keep track of recursive CTE references
	unordered_map<string, std::shared_ptr<idx_t>> cte_references;

public:
	//! Given a column name, find the matching table it belongs to. Throws an
	//! exception if no table has a column of the given name.
	string GetMatchingBinding(const string &column_name);
	//! Like GetMatchingBinding, but instead of throwing an error if multiple tables have the same binding it will
	//! return a list of all the matching ones
	unordered_set<string> GetMatchingBindings(const string &column_name);
	//! Like GetMatchingBindings, but returns the top 3 most similar bindings (in levenshtein distance) instead of the
	//! matching ones
	vector<string> GetSimilarBindings(const string &column_name);

	Binding *GetCTEBinding(const string &ctename);
	//! Binds a column expression to the base table. Returns the bound expression
	//! or throws an exception if the column could not be bound.
	BindResult BindColumn(ColumnRefExpression &colref, idx_t depth);
	string BindColumn(PositionalReferenceExpression &ref, string &table_name, string &column_name);
	BindResult BindColumn(PositionalReferenceExpression &ref, idx_t depth);

	//! Generate column expressions for all columns that are present in the
	//! referenced tables. This is used to resolve the * expression in a
	//! selection list.
	void GenerateAllColumnExpressions(vector<unique_ptr<ParsedExpression>> &new_select_list,
	                                  const string &relation_name = "");
	const vector<std::pair<string, Binding *>> &GetBindingsList() {
		return bindings_list;
	}

	//! Adds a base table with the given alias to the BindContext.
	void AddBaseTable(idx_t index, const string &alias, const vector<string> &names, const vector<LogicalType> &types,
	                  LogicalGet &get);
	//! Adds a call to a table function with the given alias to the BindContext.
	void AddTableFunction(idx_t index, const string &alias, const vector<string> &names,
	                      const vector<LogicalType> &types, LogicalGet &get);
	//! Adds a subquery with a given alias to the BindContext.
	void AddSubquery(idx_t index, const string &alias, SubqueryRef &ref, BoundQueryNode &subquery);
	//! Adds a base table with the given alias to the BindContext.
	void AddGenericBinding(idx_t index, const string &alias, const vector<string> &names,
	                       const vector<LogicalType> &types);

	//! Adds a base table with the given alias to the CTE BindContext.
	//! We need this to correctly bind recursive CTEs with multiple references.
	void AddCTEBinding(idx_t index, const string &alias, const vector<string> &names, const vector<LogicalType> &types);

	//! Add an implicit join condition (e.g. USING (x))
	void AddUsingBinding(const string &column_name, UsingColumnSet set);

	//! Returns any using column set for the given column name, or nullptr if there is none. On conflict (multiple using
	//! column sets with the same name) throw an exception.
	UsingColumnSet *GetUsingBinding(const string &column_name);
	//! Returns any using column set for the given column name, or nullptr if there is none
	UsingColumnSet *GetUsingBinding(const string &column_name, const string &binding_name);
	//! Erase a using binding from the set of using bindings
	void RemoveUsingBinding(const string &column_name, UsingColumnSet *set);

	unordered_map<string, std::shared_ptr<Binding>> GetCTEBindings() {
		return cte_bindings;
	}
	void SetCTEBindings(unordered_map<string, std::shared_ptr<Binding>> bindings) {
		cte_bindings = bindings;
	}

	//! Alias a set of column names for the specified table, using the original names if there are not enough aliases
	//! specified.
	static vector<string> AliasColumnNames(const string &table_name, const vector<string> &names,
	                                       const vector<string> &column_aliases);

	//! Add all the bindings from a BindContext to this BindContext. The other BindContext is destroyed in the process.
	void AddContext(BindContext other);

private:
	void AddBinding(const string &alias, unique_ptr<Binding> binding);
	//! Gets a binding of the specified name. Returns a nullptr and sets the out_error if the binding could not be
	//! found.
	Binding *GetBinding(const string &name, string &out_error);

private:
	//! The set of bindings
	unordered_map<string, unique_ptr<Binding>> bindings;
	//! The list of bindings in insertion order
	vector<std::pair<string, Binding *>> bindings_list;
	//! The set of columns used in USING join conditions
	unordered_map<string, vector<UsingColumnSet>> using_columns;

	//! The set of CTE bindings
	unordered_map<string, std::shared_ptr<Binding>> cte_bindings;
};
} // namespace guinsoodb

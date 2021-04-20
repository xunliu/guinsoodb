//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/main/relation.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/common.hpp"
#include "guinsoodb/common/enums/join_type.hpp"
#include "guinsoodb/common/enums/relation_type.hpp"
#include "guinsoodb/common/winapi.hpp"
#include "guinsoodb/main/query_result.hpp"
#include "guinsoodb/parser/column_definition.hpp"

#include <memory>

namespace guinsoodb {
struct BoundStatement;

class ClientContext;
class Binder;
class LogicalOperator;
class QueryNode;
class TableRef;

class Relation : public std::enable_shared_from_this<Relation> {
public:
	GUINSOODB_API Relation(ClientContext &context, RelationType type) : context(context), type(type) {
	}
	GUINSOODB_API virtual ~Relation() {
	}

	ClientContext &context;
	RelationType type;

public:
	GUINSOODB_API virtual const vector<ColumnDefinition> &Columns() = 0;
	GUINSOODB_API virtual unique_ptr<QueryNode> GetQueryNode() = 0;
	GUINSOODB_API virtual BoundStatement Bind(Binder &binder);
	GUINSOODB_API virtual string GetAlias();

	GUINSOODB_API unique_ptr<QueryResult> Execute();
	GUINSOODB_API string ToString();
	GUINSOODB_API virtual string ToString(idx_t depth) = 0;

	GUINSOODB_API void Print();
	GUINSOODB_API void Head(idx_t limit = 10);

	GUINSOODB_API shared_ptr<Relation> CreateView(const string &name, bool replace = true, bool temporary = false);
	GUINSOODB_API unique_ptr<QueryResult> Query(const string &sql);
	GUINSOODB_API unique_ptr<QueryResult> Query(const string &name, const string &sql);

	//! Explain the query plan of this relation
	GUINSOODB_API unique_ptr<QueryResult> Explain();

	GUINSOODB_API virtual unique_ptr<TableRef> GetTableRef();
	GUINSOODB_API virtual bool IsReadOnly() {
		return true;
	}

public:
	// PROJECT
	GUINSOODB_API shared_ptr<Relation> Project(const string &select_list);
	GUINSOODB_API shared_ptr<Relation> Project(const string &expression, const string &alias);
	GUINSOODB_API shared_ptr<Relation> Project(const string &select_list, const vector<string> &aliases);
	GUINSOODB_API shared_ptr<Relation> Project(const vector<string> &expressions);
	GUINSOODB_API shared_ptr<Relation> Project(const vector<string> &expressions, const vector<string> &aliases);

	// FILTER
	GUINSOODB_API shared_ptr<Relation> Filter(const string &expression);
	GUINSOODB_API shared_ptr<Relation> Filter(const vector<string> &expressions);

	// LIMIT
	GUINSOODB_API shared_ptr<Relation> Limit(int64_t n, int64_t offset = 0);

	// ORDER
	GUINSOODB_API shared_ptr<Relation> Order(const string &expression);
	GUINSOODB_API shared_ptr<Relation> Order(const vector<string> &expressions);

	// JOIN operation
	GUINSOODB_API shared_ptr<Relation> Join(const shared_ptr<Relation> &other, const string &condition,
	                                     JoinType type = JoinType::INNER);

	// SET operations
	GUINSOODB_API shared_ptr<Relation> Union(const shared_ptr<Relation> &other);
	GUINSOODB_API shared_ptr<Relation> Except(const shared_ptr<Relation> &other);
	GUINSOODB_API shared_ptr<Relation> Intersect(const shared_ptr<Relation> &other);

	// DISTINCT operation
	GUINSOODB_API shared_ptr<Relation> Distinct();

	// AGGREGATES
	GUINSOODB_API shared_ptr<Relation> Aggregate(const string &aggregate_list);
	GUINSOODB_API shared_ptr<Relation> Aggregate(const vector<string> &aggregates);
	GUINSOODB_API shared_ptr<Relation> Aggregate(const string &aggregate_list, const string &group_list);
	GUINSOODB_API shared_ptr<Relation> Aggregate(const vector<string> &aggregates, const vector<string> &groups);

	// ALIAS
	GUINSOODB_API shared_ptr<Relation> Alias(const string &alias);

	//! Insert the data from this relation into a table
	GUINSOODB_API void Insert(const string &table_name);
	GUINSOODB_API void Insert(const string &schema_name, const string &table_name);
	//! Insert a row (i.e.,list of values) into a table
	GUINSOODB_API void Insert(const vector<vector<Value>> &values);
	//! Create a table and insert the data from this relation into that table
	GUINSOODB_API void Create(const string &table_name);
	GUINSOODB_API void Create(const string &schema_name, const string &table_name);

	//! Write a relation to a CSV file
	GUINSOODB_API void WriteCSV(const string &csv_file);

	//! Update a table, can only be used on a TableRelation
	GUINSOODB_API virtual void Update(const string &update, const string &condition = string());
	//! Delete from a table, can only be used on a TableRelation
	GUINSOODB_API virtual void Delete(const string &condition = string());
	//! Create a relation from calling a table in/out function on the input relation
	GUINSOODB_API shared_ptr<Relation> TableFunction(const std::string &fname, vector<Value> &values);

public:
	//! Whether or not the relation inherits column bindings from its child or not, only relevant for binding
	GUINSOODB_API virtual bool InheritsColumnBindings() {
		return false;
	}
	GUINSOODB_API virtual Relation *ChildRelation() {
		return nullptr;
	}

protected:
	GUINSOODB_API string RenderWhitespace(idx_t depth);
};

} // namespace guinsoodb
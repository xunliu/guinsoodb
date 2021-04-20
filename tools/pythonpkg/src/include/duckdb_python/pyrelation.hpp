//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb_python/pyresult.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb_python/pybind_wrapper.hpp"
#include "guinsoodb.hpp"

namespace guinsoodb {
struct GuinsooDBPyResult;

struct GuinsooDBPyRelation {
public:
	explicit GuinsooDBPyRelation(shared_ptr<Relation> rel);

	shared_ptr<Relation> rel;

public:
	static void Initialize(py::handle &m);

	static unique_ptr<GuinsooDBPyRelation> FromDf(py::object df);

	static unique_ptr<GuinsooDBPyRelation> Values(py::object values = py::list());

	static unique_ptr<GuinsooDBPyRelation> FromCsvAuto(const string &filename);

	static unique_ptr<GuinsooDBPyRelation> FromParquet(const string &filename);

	static unique_ptr<GuinsooDBPyRelation> FromArrowTable(const py::object &table);

	unique_ptr<GuinsooDBPyRelation> Project(const string &expr);

	static unique_ptr<GuinsooDBPyRelation> ProjectDf(py::object df, const string &expr);

	unique_ptr<GuinsooDBPyRelation> Alias(const string &expr);

	static unique_ptr<GuinsooDBPyRelation> AliasDF(py::object df, const string &expr);

	unique_ptr<GuinsooDBPyRelation> Filter(const string &expr);

	static unique_ptr<GuinsooDBPyRelation> FilterDf(py::object df, const string &expr);

	unique_ptr<GuinsooDBPyRelation> Limit(int64_t n);

	static unique_ptr<GuinsooDBPyRelation> LimitDF(py::object df, int64_t n);

	unique_ptr<GuinsooDBPyRelation> Order(const string &expr);

	static unique_ptr<GuinsooDBPyRelation> OrderDf(py::object df, const string &expr);

	unique_ptr<GuinsooDBPyRelation> Aggregate(const string &expr, const string &groups = "");

	static unique_ptr<GuinsooDBPyRelation> AggregateDF(py::object df, const string &expr, const string &groups = "");

	unique_ptr<GuinsooDBPyRelation> Distinct();

	static unique_ptr<GuinsooDBPyRelation> DistinctDF(py::object df);

	py::object ToDF();

	py::object ToArrowTable();

	unique_ptr<GuinsooDBPyRelation> Union(GuinsooDBPyRelation *other);

	unique_ptr<GuinsooDBPyRelation> Except(GuinsooDBPyRelation *other);

	unique_ptr<GuinsooDBPyRelation> Intersect(GuinsooDBPyRelation *other);

	unique_ptr<GuinsooDBPyRelation> Map(py::function fun);

	unique_ptr<GuinsooDBPyRelation> Join(GuinsooDBPyRelation *other, const string &condition);

	void WriteCsv(const string &file);

	static void WriteCsvDF(py::object df, const string &file);

	// should this return a rel with the new view?
	unique_ptr<GuinsooDBPyRelation> CreateView(const string &view_name, bool replace = true);

	static unique_ptr<GuinsooDBPyRelation> CreateViewDf(py::object df, const string &view_name, bool replace = true);

	unique_ptr<GuinsooDBPyResult> Query(const string &view_name, const string &sql_query);

	unique_ptr<GuinsooDBPyResult> Execute();

	static unique_ptr<GuinsooDBPyResult> QueryDF(py::object df, const string &view_name, const string &sql_query);

	void InsertInto(const string &table);

	void Insert(py::object params = py::list());

	void Create(const string &table);

	string Print();

	py::object Getattr(const py::str &key);

private:
	py::object map_function;
};

} // namespace guinsoodb

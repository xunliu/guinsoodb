//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb_python/pyconnection.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb_python/pybind_wrapper.hpp"
#include "guinsoodb.hpp"

namespace guinsoodb {

struct GuinsooDBPyRelation;
struct GuinsooDBPyResult;

struct GuinsooDBPyConnection {
public:
	~GuinsooDBPyConnection();

	shared_ptr<GuinsooDB> database;
	unique_ptr<Connection> connection;
	unordered_map<string, py::object> registered_dfs;
	unique_ptr<GuinsooDBPyResult> result;
	vector<shared_ptr<GuinsooDBPyConnection>> cursors;

public:
	static void Initialize(py::handle &m);
	static void Cleanup();

	static GuinsooDBPyConnection *DefaultConnection();

	GuinsooDBPyConnection *ExecuteMany(const string &query, py::object params = py::list());

	GuinsooDBPyConnection *Execute(const string &query, py::object params = py::list(), bool many = false);

	GuinsooDBPyConnection *Append(const string &name, py::object value);

	GuinsooDBPyConnection *RegisterDF(const string &name, py::object value);

	unique_ptr<GuinsooDBPyRelation> Table(const string &tname);

	unique_ptr<GuinsooDBPyRelation> Values(py::object params = py::list());

	unique_ptr<GuinsooDBPyRelation> View(const string &vname);

	unique_ptr<GuinsooDBPyRelation> TableFunction(const string &fname, py::object params = py::list());

	unique_ptr<GuinsooDBPyRelation> FromDF(py::object value);

	unique_ptr<GuinsooDBPyRelation> FromCsvAuto(const string &filename);

	unique_ptr<GuinsooDBPyRelation> FromParquet(const string &filename);

	unique_ptr<GuinsooDBPyRelation> FromArrowTable(const py::object &table);

	GuinsooDBPyConnection *UnregisterDF(const string &name);

	GuinsooDBPyConnection *Begin();

	GuinsooDBPyConnection *Commit();

	GuinsooDBPyConnection *Rollback();

	py::object GetAttr(const py::str &key);

	void Close();

	// cursor() is stupid
	shared_ptr<GuinsooDBPyConnection> Cursor();

	// these should be functions on the result but well
	py::object FetchOne();

	py::list FetchAll();

	py::dict FetchNumpy();
	py::object FetchDF();

	py::object FetchDFChunk() const;

	py::object FetchArrow();

	static shared_ptr<GuinsooDBPyConnection> Connect(const string &database, bool read_only);

	static vector<Value> TransformPythonParamList(py::handle params);

private:
	//! Default connection to an in-memory database
	static shared_ptr<GuinsooDBPyConnection> default_connection;
};

} // namespace guinsoodb

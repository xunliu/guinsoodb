#include "guinsoodb_python/pyconnection.hpp"
#include "guinsoodb_python/pyresult.hpp"
#include "guinsoodb_python/pyrelation.hpp"
#include "guinsoodb_python/pandas_scan.hpp"
#include "guinsoodb_python/map.hpp"

#include "guinsoodb/common/arrow.hpp"
#include "guinsoodb/parser/parsed_data/create_table_function_info.hpp"
#include "guinsoodb/main/client_context.hpp"
#include "guinsoodb/parser/parsed_data/create_table_function_info.hpp"
#include "guinsoodb/common/types/vector.hpp"
#include "guinsoodb/common/printer.hpp"
#include "guinsoodb/main/config.hpp"
#include "guinsoodb/parser/expression/constant_expression.hpp"
#include "guinsoodb/parser/expression/function_expression.hpp"
#include "guinsoodb/parser/tableref/table_function_ref.hpp"

#include "extension/extension_helper.hpp"

#include "datetime.h" // from Python

#include <random>

namespace guinsoodb {

shared_ptr<GuinsooDBPyConnection> GuinsooDBPyConnection::default_connection = nullptr;

void GuinsooDBPyConnection::Initialize(py::handle &m) {
	py::class_<GuinsooDBPyConnection, shared_ptr<GuinsooDBPyConnection>>(m, "GuinsooDBPyConnection")
	    .def("cursor", &GuinsooDBPyConnection::Cursor, "Create a duplicate of the current connection")
	    .def("duplicate", &GuinsooDBPyConnection::Cursor, "Create a duplicate of the current connection")
	    .def("execute", &GuinsooDBPyConnection::Execute,
	         "Execute the given SQL query, optionally using prepared statements with parameters set", py::arg("query"),
	         py::arg("parameters") = py::list(), py::arg("multiple_parameter_sets") = false)
	    .def("executemany", &GuinsooDBPyConnection::ExecuteMany,
	         "Execute the given prepared statement multiple times using the list of parameter sets in parameters",
	         py::arg("query"), py::arg("parameters") = py::list())
	    .def("close", &GuinsooDBPyConnection::Close, "Close the connection")
	    .def("fetchone", &GuinsooDBPyConnection::FetchOne, "Fetch a single row from a result following execute")
	    .def("fetchall", &GuinsooDBPyConnection::FetchAll, "Fetch all rows from a result following execute")
	    .def("fetchnumpy", &GuinsooDBPyConnection::FetchNumpy, "Fetch a result as list of NumPy arrays following execute")
	    .def("fetchdf", &GuinsooDBPyConnection::FetchDF, "Fetch a result as Data.Frame following execute()")
	    .def("fetch_df", &GuinsooDBPyConnection::FetchDF, "Fetch a result as Data.Frame following execute()")
	    .def("fetch_df_chunk", &GuinsooDBPyConnection::FetchDFChunk,
	         "Fetch a chunk of the result as Data.Frame following execute()")
	    .def("df", &GuinsooDBPyConnection::FetchDF, "Fetch a result as Data.Frame following execute()")
	    .def("fetch_arrow_table", &GuinsooDBPyConnection::FetchArrow, "Fetch a result as Arrow table following execute()")
	    .def("arrow", &GuinsooDBPyConnection::FetchArrow, "Fetch a result as Arrow table following execute()")
	    .def("begin", &GuinsooDBPyConnection::Begin, "Start a new transaction")
	    .def("commit", &GuinsooDBPyConnection::Commit, "Commit changes performed within a transaction")
	    .def("rollback", &GuinsooDBPyConnection::Rollback, "Roll back changes performed within a transaction")
	    .def("append", &GuinsooDBPyConnection::Append, "Append the passed Data.Frame to the named table",
	         py::arg("table_name"), py::arg("df"))
	    .def("register", &GuinsooDBPyConnection::RegisterDF,
	         "Register the passed Data.Frame value for querying with a view", py::arg("view_name"), py::arg("df"))
	    .def("unregister", &GuinsooDBPyConnection::UnregisterDF, "Unregister the view name", py::arg("view_name"))
	    .def("table", &GuinsooDBPyConnection::Table, "Create a relation object for the name'd table",
	         py::arg("table_name"))
	    .def("view", &GuinsooDBPyConnection::View, "Create a relation object for the name'd view", py::arg("view_name"))
	    .def("values", &GuinsooDBPyConnection::Values, "Create a relation object from the passed values",
	         py::arg("values"))
	    .def("table_function", &GuinsooDBPyConnection::TableFunction,
	         "Create a relation object from the name'd table function with given parameters", py::arg("name"),
	         py::arg("parameters") = py::list())
	    .def("from_df", &GuinsooDBPyConnection::FromDF, "Create a relation object from the Data.Frame in df",
	         py::arg("df") = py::none())
	    .def("from_arrow_table", &GuinsooDBPyConnection::FromArrowTable, "Create a relation object from an Arrow table",
	         py::arg("table"))
	    .def("df", &GuinsooDBPyConnection::FromDF, "Create a relation object from the Data.Frame in df (alias of from_df)",
	         py::arg("df"))
	    .def("from_csv_auto", &GuinsooDBPyConnection::FromCsvAuto,
	         "Create a relation object from the CSV file in file_name", py::arg("file_name"))
	    .def("from_parquet", &GuinsooDBPyConnection::FromParquet,
	         "Create a relation object from the Parquet file in file_name", py::arg("file_name"))
	    .def("__getattr__", &GuinsooDBPyConnection::GetAttr, "Get result set attributes, mainly column names");

	PyDateTime_IMPORT;
}

GuinsooDBPyConnection::~GuinsooDBPyConnection() {
	for (auto &element : registered_dfs) {
		UnregisterDF(element.first);
	}
}

GuinsooDBPyConnection *GuinsooDBPyConnection::ExecuteMany(const string &query, py::object params) {
	Execute(query, std::move(params), true);
	return this;
}

GuinsooDBPyConnection *GuinsooDBPyConnection::Execute(const string &query, py::object params, bool many) {
	if (!connection) {
		throw std::runtime_error("connection closed");
	}
	result = nullptr;

	auto statements = connection->ExtractStatements(query);
	if (statements.empty()) {
		// no statements to execute
		return this;
	}
	// if there are multiple statements, we directly execute the statements besides the last one
	// we only return the result of the last statement to the user, unless one of the previous statements fails
	for (idx_t i = 0; i + 1 < statements.size(); i++) {
		auto res = connection->Query(move(statements[i]));
		if (!res->success) {
			throw std::runtime_error(res->error);
		}
	}

	auto prep = connection->Prepare(move(statements.back()));
	if (!prep->success) {
		throw std::runtime_error(prep->error);
	}

	// this is a list of a list of parameters in executemany
	py::list params_set;
	if (!many) {
		params_set = py::list(1);
		params_set[0] = params;
	} else {
		params_set = params;
	}

	for (pybind11::handle single_query_params : params_set) {
		if (prep->n_param != py::len(single_query_params)) {
			throw std::runtime_error("Prepared statement needs " + to_string(prep->n_param) + " parameters, " +
			                         to_string(py::len(single_query_params)) + " given");
		}
		auto args = GuinsooDBPyConnection::TransformPythonParamList(single_query_params);
		auto res = make_unique<GuinsooDBPyResult>();
		{
			py::gil_scoped_release release;
			res->result = prep->Execute(args);
		}
		if (!res->result->success) {
			throw std::runtime_error(res->result->error);
		}
		if (!many) {
			result = move(res);
		}
	}
	return this;
}

GuinsooDBPyConnection *GuinsooDBPyConnection::Append(const string &name, py::object value) {
	RegisterDF("__append_df", std::move(value));
	return Execute("INSERT INTO \"" + name + "\" SELECT * FROM __append_df");
}

GuinsooDBPyConnection *GuinsooDBPyConnection::RegisterDF(const string &name, py::object value) {
	if (!connection) {
		throw std::runtime_error("connection closed");
	}
	connection->TableFunction("pandas_scan", {Value::POINTER((uintptr_t)value.ptr())})->CreateView(name, true, true);
	// keep a reference
	registered_dfs[name] = value;
	return this;
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyConnection::Table(const string &tname) {
	if (!connection) {
		throw std::runtime_error("connection closed");
	}
	return make_unique<GuinsooDBPyRelation>(connection->Table(tname));
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyConnection::Values(py::object params) {
	if (!connection) {
		throw std::runtime_error("connection closed");
	}
	vector<vector<Value>> values {GuinsooDBPyConnection::TransformPythonParamList(std::move(params))};
	return make_unique<GuinsooDBPyRelation>(connection->Values(values));
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyConnection::View(const string &vname) {
	if (!connection) {
		throw std::runtime_error("connection closed");
	}
	return make_unique<GuinsooDBPyRelation>(connection->View(vname));
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyConnection::TableFunction(const string &fname, py::object params) {
	if (!connection) {
		throw std::runtime_error("connection closed");
	}

	return make_unique<GuinsooDBPyRelation>(
	    connection->TableFunction(fname, GuinsooDBPyConnection::TransformPythonParamList(std::move(params))));
}

static std::string GenerateRandomName() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 15);

	std::stringstream ss;
	int i;
	ss << std::hex;
	for (i = 0; i < 16; i++) {
		ss << dis(gen);
	}
	return ss.str();
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyConnection::FromDF(py::object value) {
	if (!connection) {
		throw std::runtime_error("connection closed");
	}
	string name = "df_" + GenerateRandomName();
	registered_dfs[name] = value;
	vector<Value> params;
	params.emplace_back(Value::POINTER((uintptr_t)value.ptr()));
	return make_unique<GuinsooDBPyRelation>(connection->TableFunction("pandas_scan", params)->Alias(name));
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyConnection::FromCsvAuto(const string &filename) {
	if (!connection) {
		throw std::runtime_error("connection closed");
	}
	vector<Value> params;
	params.emplace_back(filename);
	return make_unique<GuinsooDBPyRelation>(connection->TableFunction("read_csv_auto", params)->Alias(filename));
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyConnection::FromParquet(const string &filename) {
	if (!connection) {
		throw std::runtime_error("connection closed");
	}
	vector<Value> params;
	params.emplace_back(filename);
	return make_unique<GuinsooDBPyRelation>(connection->TableFunction("parquet_scan", params)->Alias(filename));
}

struct PythonTableArrowArrayStream {
	explicit PythonTableArrowArrayStream(const py::object &arrow_table) : arrow_table(arrow_table) {
		stream.get_schema = PythonTableArrowArrayStream::MyStreamGetSchema;
		stream.get_next = PythonTableArrowArrayStream::MyStreamGetNext;
		stream.release = PythonTableArrowArrayStream::MyStreamRelease;
		stream.get_last_error = PythonTableArrowArrayStream::MyStreamGetLastError;
		stream.private_data = this;

		batches = arrow_table.attr("to_batches")();
	}

	static int MyStreamGetSchema(struct ArrowArrayStream *stream, struct ArrowSchema *out) {
		D_ASSERT(stream->private_data);
		auto my_stream = (PythonTableArrowArrayStream *)stream->private_data;
		if (!stream->release) {
			my_stream->last_error = "stream was released";
			return -1;
		}
		auto schema = my_stream->arrow_table.attr("schema");
		if (!py::hasattr(schema, "_export_to_c")) {
			my_stream->last_error = "failed to acquire export_to_c function";
			return -1;
		}
		auto export_to_c = schema.attr("_export_to_c");
		export_to_c((uint64_t)out);
		return 0;
	}

	static int MyStreamGetNext(struct ArrowArrayStream *stream, struct ArrowArray *out) {
		D_ASSERT(stream->private_data);
		auto my_stream = (PythonTableArrowArrayStream *)stream->private_data;
		if (!stream->release) {
			my_stream->last_error = "stream was released";
			return -1;
		}
		if (my_stream->batch_idx >= py::len(my_stream->batches)) {
			out->release = nullptr;
			return 0;
		}
		auto stream_batch = my_stream->batches[my_stream->batch_idx++];
		if (!py::hasattr(stream_batch, "_export_to_c")) {
			my_stream->last_error = "failed to acquire export_to_c function";
			return -1;
		}
		auto export_to_c = stream_batch.attr("_export_to_c");
		export_to_c((uint64_t)out);
		return 0;
	}

	static void MyStreamRelease(struct ArrowArrayStream *stream) {
		if (!stream->release) {
			return;
		}
		stream->release = nullptr;
		delete (PythonTableArrowArrayStream *)stream->private_data;
	}

	static const char *MyStreamGetLastError(struct ArrowArrayStream *stream) {
		if (!stream->release) {
			return "stream was released";
		}
		D_ASSERT(stream->private_data);
		auto my_stream = (PythonTableArrowArrayStream *)stream->private_data;
		return my_stream->last_error.c_str();
	}

	ArrowArrayStream stream;
	string last_error;
	py::object arrow_table;
	py::list batches;
	idx_t batch_idx = 0;
};

static string PtrToString(void const *ptr) {
	std::ostringstream address;
	address << ptr;
	return address.str();
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyConnection::FromArrowTable(const py::object &table) {
	if (!connection) {
		throw std::runtime_error("connection closed");
	}

	// the following is a careful dance around having to depend on pyarrow
	if (table.is_none() || string(py::str(table.get_type().attr("__name__"))) != "Table") {
		throw std::runtime_error("Only arrow tables supported");
	}

	auto my_arrow_table = new PythonTableArrowArrayStream(table);
	string name = "arrow_table_" + PtrToString((void *)my_arrow_table);
	return make_unique<GuinsooDBPyRelation>(
	    connection->TableFunction("arrow_scan", {Value::POINTER((uintptr_t)my_arrow_table)})->Alias(name));
}

GuinsooDBPyConnection *GuinsooDBPyConnection::UnregisterDF(const string &name) {
	registered_dfs[name] = py::none();
	return this;
}

GuinsooDBPyConnection *GuinsooDBPyConnection::Begin() {
	Execute("BEGIN TRANSACTION");
	return this;
}

GuinsooDBPyConnection *GuinsooDBPyConnection::Commit() {
	if (connection->context->transaction.IsAutoCommit()) {
		return this;
	}
	Execute("COMMIT");
	return this;
}

GuinsooDBPyConnection *GuinsooDBPyConnection::Rollback() {
	Execute("ROLLBACK");
	return this;
}

py::object GuinsooDBPyConnection::GetAttr(const py::str &key) {
	if (key.cast<string>() == "description") {
		if (!result) {
			throw std::runtime_error("no open result set");
		}
		return result->Description();
	}
	return py::none();
}

void GuinsooDBPyConnection::Close() {
	result = nullptr;
	connection = nullptr;
	database = nullptr;
	for (auto &cur : cursors) {
		cur->Close();
	}
	cursors.clear();
}

// cursor() is stupid
shared_ptr<GuinsooDBPyConnection> GuinsooDBPyConnection::Cursor() {
	auto res = make_shared<GuinsooDBPyConnection>();
	res->database = database;
	res->connection = make_unique<Connection>(*res->database);
	cursors.push_back(res);
	return res;
}

// these should be functions on the result but well
py::object GuinsooDBPyConnection::FetchOne() {
	if (!result) {
		throw std::runtime_error("no open result set");
	}
	return result->Fetchone();
}

py::list GuinsooDBPyConnection::FetchAll() {
	if (!result) {
		throw std::runtime_error("no open result set");
	}
	return result->Fetchall();
}

py::dict GuinsooDBPyConnection::FetchNumpy() {
	if (!result) {
		throw std::runtime_error("no open result set");
	}
	return result->FetchNumpy();
}
py::object GuinsooDBPyConnection::FetchDF() {
	if (!result) {
		throw std::runtime_error("no open result set");
	}
	return result->FetchDF();
}

py::object GuinsooDBPyConnection::FetchDFChunk() const {
	if (!result) {
		throw std::runtime_error("no open result set");
	}
	return result->FetchDFChunk();
}

py::object GuinsooDBPyConnection::FetchArrow() {
	if (!result) {
		throw std::runtime_error("no open result set");
	}
	return result->FetchArrowTable();
}

static unique_ptr<TableFunctionRef> TryPandasReplacement(py::dict &dict, py::str &table_name) {
	if (!dict.contains(table_name)) {
		// not present in the globals
		return nullptr;
	}
	auto entry = dict[table_name];

	// check if there is a local or global variable
	auto table_function = make_unique<TableFunctionRef>();
	vector<unique_ptr<ParsedExpression>> children;
	children.push_back(make_unique<ConstantExpression>(Value::POINTER((uintptr_t)entry.ptr())));
	table_function->function = make_unique<FunctionExpression>("pandas_scan", children);
	return table_function;
}

static unique_ptr<TableFunctionRef> PandasScanReplacement(const string &table_name, void *data) {
	// look in the locals first
	PyObject *p = PyEval_GetLocals();
	auto py_table_name = py::str(table_name);
	if (p) {
		auto local_dict = py::reinterpret_borrow<py::dict>(p);
		auto result = TryPandasReplacement(local_dict, py_table_name);
		if (result) {
			return result;
		}
	}
	// otherwise look in the globals
	auto global_dict = py::globals();
	return TryPandasReplacement(global_dict, py_table_name);
}

shared_ptr<GuinsooDBPyConnection> GuinsooDBPyConnection::Connect(const string &database, bool read_only) {
	auto res = make_shared<GuinsooDBPyConnection>();
	DBConfig config;
	if (read_only) {
		config.access_mode = AccessMode::READ_ONLY;
	}
	config.replacement_scans.emplace_back(PandasScanReplacement);

	res->database = make_unique<GuinsooDB>(database, &config);
	ExtensionHelper::LoadAllExtensions(*res->database);
	res->connection = make_unique<Connection>(*res->database);

	PandasScanFunction scan_fun;
	CreateTableFunctionInfo scan_info(scan_fun);

	MapFunction map_fun;
	CreateTableFunctionInfo map_info(map_fun);

	auto &context = *res->connection->context;
	auto &catalog = Catalog::GetCatalog(context);
	context.transaction.BeginTransaction();
	catalog.CreateTableFunction(context, &scan_info);
	catalog.CreateTableFunction(context, &map_info);

	context.transaction.Commit();

	return res;
}

vector<Value> GuinsooDBPyConnection::TransformPythonParamList(py::handle params) {
	vector<Value> args;

	auto datetime_mod = py::module::import("datetime");
	auto datetime_date = datetime_mod.attr("date");
	auto datetime_datetime = datetime_mod.attr("datetime");
	auto datetime_time = datetime_mod.attr("time");
	auto decimal_mod = py::module::import("decimal");
	auto decimal_decimal = decimal_mod.attr("Decimal");

	for (pybind11::handle ele : params) {
		if (ele.is_none()) {
			args.emplace_back();
		} else if (py::isinstance<py::bool_>(ele)) {
			args.push_back(Value::BOOLEAN(ele.cast<bool>()));
		} else if (py::isinstance<py::int_>(ele)) {
			args.push_back(Value::BIGINT(ele.cast<int64_t>()));
		} else if (py::isinstance<py::float_>(ele)) {
			args.push_back(Value::DOUBLE(ele.cast<double>()));
		} else if (py::isinstance<py::str>(ele)) {
			args.emplace_back(ele.cast<string>());
		} else if (py::isinstance(ele, decimal_decimal)) {
			args.emplace_back(py::str(ele).cast<string>());
		} else if (py::isinstance(ele, datetime_datetime)) {
			auto year = PyDateTime_GET_YEAR(ele.ptr());
			auto month = PyDateTime_GET_MONTH(ele.ptr());
			auto day = PyDateTime_GET_DAY(ele.ptr());
			auto hour = PyDateTime_DATE_GET_HOUR(ele.ptr());
			auto minute = PyDateTime_DATE_GET_MINUTE(ele.ptr());
			auto second = PyDateTime_DATE_GET_SECOND(ele.ptr());
			auto micros = PyDateTime_DATE_GET_MICROSECOND(ele.ptr());
			args.push_back(Value::TIMESTAMP(year, month, day, hour, minute, second, micros));
		} else if (py::isinstance(ele, datetime_time)) {
			auto hour = PyDateTime_TIME_GET_HOUR(ele.ptr());
			auto minute = PyDateTime_TIME_GET_MINUTE(ele.ptr());
			auto second = PyDateTime_TIME_GET_SECOND(ele.ptr());
			auto micros = PyDateTime_TIME_GET_MICROSECOND(ele.ptr());
			args.push_back(Value::TIME(hour, minute, second, micros));
		} else if (py::isinstance(ele, datetime_date)) {
			auto year = PyDateTime_GET_YEAR(ele.ptr());
			auto month = PyDateTime_GET_MONTH(ele.ptr());
			auto day = PyDateTime_GET_DAY(ele.ptr());
			args.push_back(Value::DATE(year, month, day));
		} else {
			throw std::runtime_error("unknown param type " + py::str(ele.get_type()).cast<string>());
		}
	}
	return args;
}

GuinsooDBPyConnection *GuinsooDBPyConnection::DefaultConnection() {
	if (!default_connection) {
		default_connection = GuinsooDBPyConnection::Connect(":memory:", false);
	}
	return default_connection.get();
}

void GuinsooDBPyConnection::Cleanup() {
	default_connection.reset();
}

} // namespace guinsoodb

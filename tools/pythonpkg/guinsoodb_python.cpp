#include "guinsoodb_python/pybind_wrapper.hpp"

#include <unordered_map>
#include <vector>
#include <atomic>

#include "guinsoodb_python/array_wrapper.hpp"
#include "guinsoodb_python/pandas_scan.hpp"
#include "guinsoodb_python/pyconnection.hpp"
#include "guinsoodb_python/pyrelation.hpp"
#include "guinsoodb_python/pyresult.hpp"
#include "guinsoodb/parser/parser.hpp"

#include "datetime.h" // from Python

#include "guinsoodb.hpp"
#include <random>
#include <stdlib.h>

namespace py = pybind11;

namespace guinsoodb {

enum PySQLTokenType {
	PY_SQL_TOKEN_IDENTIFIER = 0,
	PY_SQL_TOKEN_NUMERIC_CONSTANT,
	PY_SQL_TOKEN_STRING_CONSTANT,
	PY_SQL_TOKEN_OPERATOR,
	PY_SQL_TOKEN_KEYWORD,
	PY_SQL_TOKEN_COMMENT
};

static py::object PyTokenize(const string &query) {
	auto tokens = Parser::Tokenize(query);
	py::list result;
	for (auto &token : tokens) {
		auto tuple = py::tuple(2);
		tuple[0] = token.start;
		switch (token.type) {
		case SimplifiedTokenType::SIMPLIFIED_TOKEN_IDENTIFIER:
			tuple[1] = PY_SQL_TOKEN_IDENTIFIER;
			break;
		case SimplifiedTokenType::SIMPLIFIED_TOKEN_NUMERIC_CONSTANT:
			tuple[1] = PY_SQL_TOKEN_NUMERIC_CONSTANT;
			break;
		case SimplifiedTokenType::SIMPLIFIED_TOKEN_STRING_CONSTANT:
			tuple[1] = PY_SQL_TOKEN_STRING_CONSTANT;
			break;
		case SimplifiedTokenType::SIMPLIFIED_TOKEN_OPERATOR:
			tuple[1] = PY_SQL_TOKEN_OPERATOR;
			break;
		case SimplifiedTokenType::SIMPLIFIED_TOKEN_KEYWORD:
			tuple[1] = PY_SQL_TOKEN_KEYWORD;
			break;
		case SimplifiedTokenType::SIMPLIFIED_TOKEN_COMMENT:
			tuple[1] = PY_SQL_TOKEN_COMMENT;
			break;
		}
		result.append(tuple);
	}
	return move(result);
}

PYBIND11_MODULE(guinsoodb, m) {
	GuinsooDBPyRelation::Initialize(m);
	GuinsooDBPyResult::Initialize(m);
	GuinsooDBPyConnection::Initialize(m);

	m.doc() = "GuinsooDB is an embeddable SQL OLAP Database Management System";
	m.attr("__package__") = "guinsoodb";
	m.attr("__version__") = GuinsooDB::LibraryVersion();
	m.attr("__git_revision__") = GuinsooDB::SourceID();

	m.def("connect", &GuinsooDBPyConnection::Connect,
	      "Create a GuinsooDB database instance. Can take a database file name to read/write persistent data and a "
	      "read_only flag if no changes are desired",
	      py::arg("database") = ":memory:", py::arg("read_only") = false);
	m.def("tokenize", PyTokenize,
	      "Tokenizes a SQL string, returning a list of (position, type) tuples that can be "
	      "used for e.g. syntax highlighting",
	      py::arg("query"));
	py::enum_<PySQLTokenType>(m, "token_type")
	    .value("identifier", PySQLTokenType::PY_SQL_TOKEN_IDENTIFIER)
	    .value("numeric_const", PySQLTokenType::PY_SQL_TOKEN_NUMERIC_CONSTANT)
	    .value("string_const", PySQLTokenType::PY_SQL_TOKEN_STRING_CONSTANT)
	    .value("operator", PySQLTokenType::PY_SQL_TOKEN_OPERATOR)
	    .value("keyword", PySQLTokenType::PY_SQL_TOKEN_KEYWORD)
	    .value("comment", PySQLTokenType::PY_SQL_TOKEN_COMMENT)
	    .export_values();

	m.def("values", &GuinsooDBPyRelation::Values, "Create a relation object from the passed values", py::arg("values"));
	m.def("from_csv_auto", &GuinsooDBPyRelation::FromCsvAuto, "Creates a relation object from the CSV file in file_name",
	      py::arg("file_name"));
	m.def("from_parquet", &GuinsooDBPyRelation::FromParquet,
	      "Creates a relation object from the Parquet file in file_name", py::arg("file_name"));
	m.def("df", &GuinsooDBPyRelation::FromDf, "Create a relation object from the Data.Frame df", py::arg("df"));
	m.def("from_df", &GuinsooDBPyRelation::FromDf, "Create a relation object from the Data.Frame df", py::arg("df"));
	m.def("from_arrow_table", &GuinsooDBPyRelation::FromArrowTable, "Create a relation object from an Arrow table",
	      py::arg("table"));
	m.def("arrow", &GuinsooDBPyRelation::FromArrowTable, "Create a relation object from an Arrow table", py::arg("table"));
	m.def("filter", &GuinsooDBPyRelation::FilterDf, "Filter the Data.Frame df by the filter in filter_expr", py::arg("df"),
	      py::arg("filter_expr"));
	m.def("project", &GuinsooDBPyRelation::ProjectDf, "Project the Data.Frame df by the projection in project_expr",
	      py::arg("df"), py::arg("project_expr"));
	m.def("alias", &GuinsooDBPyRelation::AliasDF, "Create a relation from Data.Frame df with the passed alias",
	      py::arg("df"), py::arg("alias"));
	m.def("order", &GuinsooDBPyRelation::OrderDf, "Reorder the Data.Frame df by order_expr", py::arg("df"),
	      py::arg("order_expr"));
	m.def("aggregate", &GuinsooDBPyRelation::AggregateDF,
	      "Compute the aggregate aggr_expr by the optional groups group_expr on Data.frame df", py::arg("df"),
	      py::arg("aggr_expr"), py::arg("group_expr") = "");
	m.def("distinct", &GuinsooDBPyRelation::DistinctDF, "Compute the distinct rows from Data.Frame df ", py::arg("df"));
	m.def("limit", &GuinsooDBPyRelation::LimitDF, "Retrieve the first n rows from the Data.Frame df", py::arg("df"),
	      py::arg("n"));
	m.def("query", &GuinsooDBPyRelation::QueryDF,
	      "Run the given SQL query in sql_query on the view named virtual_table_name that contains the content of "
	      "Data.Frame df",
	      py::arg("df"), py::arg("virtual_table_name"), py::arg("sql_query"));
	m.def("write_csv", &GuinsooDBPyRelation::WriteCsvDF, "Write the Data.Frame df to a CSV file in file_name",
	      py::arg("df"), py::arg("file_name"));

	// we need this because otherwise we try to remove registered_dfs on shutdown when python is already dead
	auto clean_default_connection = []() {
		GuinsooDBPyConnection::Cleanup();
	};
	m.add_object("_clean_default_connection", py::capsule(clean_default_connection));

	PyDateTime_IMPORT;
}

} // namespace guinsoodb

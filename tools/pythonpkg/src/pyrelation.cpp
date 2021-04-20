#include "guinsoodb_python/pyrelation.hpp"
#include "guinsoodb_python/pyconnection.hpp"
#include "guinsoodb_python/pyresult.hpp"

namespace guinsoodb {

void GuinsooDBPyRelation::Initialize(py::handle &m) {
	py::class_<GuinsooDBPyRelation>(m, "GuinsooDBPyRelation")
	    .def("filter", &GuinsooDBPyRelation::Filter, "Filter the relation object by the filter in filter_expr",
	         py::arg("filter_expr"))
	    .def("project", &GuinsooDBPyRelation::Project, "Project the relation object by the projection in project_expr",
	         py::arg("project_expr"))
	    .def("set_alias", &GuinsooDBPyRelation::Alias, "Rename the relation object to new alias", py::arg("alias"))
	    .def("order", &GuinsooDBPyRelation::Order, "Reorder the relation object by order_expr", py::arg("order_expr"))
	    .def("aggregate", &GuinsooDBPyRelation::Aggregate,
	         "Compute the aggregate aggr_expr by the optional groups group_expr on the relation", py::arg("aggr_expr"),
	         py::arg("group_expr") = "")
	    .def("union", &GuinsooDBPyRelation::Union,
	         "Create the set union of this relation object with another relation object in other_rel")
	    .def("except_", &GuinsooDBPyRelation::Except,
	         "Create the set except of this relation object with another relation object in other_rel",
	         py::arg("other_rel"))
	    .def("intersect", &GuinsooDBPyRelation::Intersect,
	         "Create the set intersection of this relation object with another relation object in other_rel",
	         py::arg("other_rel"))
	    .def("join", &GuinsooDBPyRelation::Join,
	         "Join the relation object with another relation object in other_rel using the join condition expression "
	         "in join_condition",
	         py::arg("other_rel"), py::arg("join_condition"))
	    .def("distinct", &GuinsooDBPyRelation::Distinct, "Retrieve distinct rows from this relation object")
	    .def("limit", &GuinsooDBPyRelation::Limit, "Only retrieve the first n rows from this relation object",
	         py::arg("n"))
	    .def("query", &GuinsooDBPyRelation::Query,
	         "Run the given SQL query in sql_query on the view named virtual_table_name that refers to the relation "
	         "object",
	         py::arg("virtual_table_name"), py::arg("sql_query"))
	    .def("execute", &GuinsooDBPyRelation::Execute, "Transform the relation into a result set")
	    .def("write_csv", &GuinsooDBPyRelation::WriteCsv, "Write the relation object to a CSV file in file_name",
	         py::arg("file_name"))
	    .def("insert_into", &GuinsooDBPyRelation::InsertInto,
	         "Inserts the relation object into an existing table named table_name", py::arg("table_name"))
	    .def("insert", &GuinsooDBPyRelation::Insert, "Inserts the given values into the relation", py::arg("values"))
	    .def("create", &GuinsooDBPyRelation::Create,
	         "Creates a new table named table_name with the contents of the relation object", py::arg("table_name"))
	    .def("create_view", &GuinsooDBPyRelation::CreateView,
	         "Creates a view named view_name that refers to the relation object", py::arg("view_name"),
	         py::arg("replace") = true)
	    .def("to_arrow_table", &GuinsooDBPyRelation::ToArrowTable, "Transforms the relation object into a Arrow table")
	    .def("arrow", &GuinsooDBPyRelation::ToArrowTable, "Transforms the relation object into a Arrow table")
	    .def("to_df", &GuinsooDBPyRelation::ToDF, "Transforms the relation object into a Data.Frame")
	    .def("df", &GuinsooDBPyRelation::ToDF, "Transforms the relation object into a Data.Frame")
	    .def("map", &GuinsooDBPyRelation::Map, py::arg("map_function"), "Calls the passed function on the relation")
	    .def("__str__", &GuinsooDBPyRelation::Print)
	    .def("__repr__", &GuinsooDBPyRelation::Print)
	    .def("__getattr__", &GuinsooDBPyRelation::Getattr);
}

GuinsooDBPyRelation::GuinsooDBPyRelation(shared_ptr<Relation> rel) : rel(move(rel)) {
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyRelation::FromDf(py::object df) {
	return GuinsooDBPyConnection::DefaultConnection()->FromDF(std::move(df));
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyRelation::Values(py::object values) {
	return GuinsooDBPyConnection::DefaultConnection()->Values(std::move(values));
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyRelation::FromCsvAuto(const string &filename) {
	return GuinsooDBPyConnection::DefaultConnection()->FromCsvAuto(filename);
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyRelation::FromParquet(const string &filename) {
	return GuinsooDBPyConnection::DefaultConnection()->FromParquet(filename);
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyRelation::FromArrowTable(const py::object &table) {
	return GuinsooDBPyConnection::DefaultConnection()->FromArrowTable(table);
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyRelation::Project(const string &expr) {
	return make_unique<GuinsooDBPyRelation>(rel->Project(expr));
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyRelation::ProjectDf(py::object df, const string &expr) {
	return GuinsooDBPyConnection::DefaultConnection()->FromDF(std::move(df))->Project(expr);
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyRelation::Alias(const string &expr) {
	return make_unique<GuinsooDBPyRelation>(rel->Alias(expr));
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyRelation::AliasDF(py::object df, const string &expr) {
	return GuinsooDBPyConnection::DefaultConnection()->FromDF(std::move(df))->Alias(expr);
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyRelation::Filter(const string &expr) {
	return make_unique<GuinsooDBPyRelation>(rel->Filter(expr));
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyRelation::FilterDf(py::object df, const string &expr) {
	return GuinsooDBPyConnection::DefaultConnection()->FromDF(std::move(df))->Filter(expr);
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyRelation::Limit(int64_t n) {
	return make_unique<GuinsooDBPyRelation>(rel->Limit(n));
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyRelation::LimitDF(py::object df, int64_t n) {
	return GuinsooDBPyConnection::DefaultConnection()->FromDF(std::move(df))->Limit(n);
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyRelation::Order(const string &expr) {
	return make_unique<GuinsooDBPyRelation>(rel->Order(expr));
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyRelation::OrderDf(py::object df, const string &expr) {
	return GuinsooDBPyConnection::DefaultConnection()->FromDF(std::move(df))->Order(expr);
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyRelation::Aggregate(const string &expr, const string &groups) {
	if (!groups.empty()) {
		return make_unique<GuinsooDBPyRelation>(rel->Aggregate(expr, groups));
	}
	return make_unique<GuinsooDBPyRelation>(rel->Aggregate(expr));
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyRelation::AggregateDF(py::object df, const string &expr, const string &groups) {
	return GuinsooDBPyConnection::DefaultConnection()->FromDF(std::move(df))->Aggregate(expr, groups);
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyRelation::Distinct() {
	return make_unique<GuinsooDBPyRelation>(rel->Distinct());
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyRelation::DistinctDF(py::object df) {
	return GuinsooDBPyConnection::DefaultConnection()->FromDF(std::move(df))->Distinct();
}

py::object GuinsooDBPyRelation::ToDF() {
	auto res = make_unique<GuinsooDBPyResult>();
	{
		py::gil_scoped_release release;
		res->result = rel->Execute();
	}
	if (!res->result->success) {
		throw std::runtime_error(res->result->error);
	}
	return res->FetchDF();
}

py::object GuinsooDBPyRelation::ToArrowTable() {
	auto res = make_unique<GuinsooDBPyResult>();
	{
		py::gil_scoped_release release;
		res->result = rel->Execute();
	}
	if (!res->result->success) {
		throw std::runtime_error(res->result->error);
	}
	return res->FetchArrowTable();
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyRelation::Union(GuinsooDBPyRelation *other) {
	return make_unique<GuinsooDBPyRelation>(rel->Union(other->rel));
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyRelation::Except(GuinsooDBPyRelation *other) {
	return make_unique<GuinsooDBPyRelation>(rel->Except(other->rel));
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyRelation::Intersect(GuinsooDBPyRelation *other) {
	return make_unique<GuinsooDBPyRelation>(rel->Intersect(other->rel));
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyRelation::Join(GuinsooDBPyRelation *other, const string &condition) {
	return make_unique<GuinsooDBPyRelation>(rel->Join(other->rel, condition));
}

void GuinsooDBPyRelation::WriteCsv(const string &file) {
	rel->WriteCSV(file);
}

void GuinsooDBPyRelation::WriteCsvDF(py::object df, const string &file) {
	return GuinsooDBPyConnection::DefaultConnection()->FromDF(std::move(df))->WriteCsv(file);
}

// should this return a rel with the new view?
unique_ptr<GuinsooDBPyRelation> GuinsooDBPyRelation::CreateView(const string &view_name, bool replace) {
	rel->CreateView(view_name, replace);
	return make_unique<GuinsooDBPyRelation>(rel);
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyRelation::CreateViewDf(py::object df, const string &view_name, bool replace) {
	return GuinsooDBPyConnection::DefaultConnection()->FromDF(std::move(df))->CreateView(view_name, replace);
}

unique_ptr<GuinsooDBPyResult> GuinsooDBPyRelation::Query(const string &view_name, const string &sql_query) {
	auto res = make_unique<GuinsooDBPyResult>();
	res->result = rel->Query(view_name, sql_query);
	if (!res->result->success) {
		throw std::runtime_error(res->result->error);
	}
	return res;
}

unique_ptr<GuinsooDBPyResult> GuinsooDBPyRelation::Execute() {
	auto res = make_unique<GuinsooDBPyResult>();
	{
		py::gil_scoped_release release;
		res->result = rel->Execute();
	}
	if (!res->result->success) {
		throw std::runtime_error(res->result->error);
	}
	return res;
}

unique_ptr<GuinsooDBPyResult> GuinsooDBPyRelation::QueryDF(py::object df, const string &view_name, const string &sql_query) {
	return GuinsooDBPyConnection::DefaultConnection()->FromDF(std::move(df))->Query(view_name, sql_query);
}

void GuinsooDBPyRelation::InsertInto(const string &table) {
	rel->Insert(table);
}

void GuinsooDBPyRelation::Insert(py::object params) {
	vector<vector<Value>> values {GuinsooDBPyConnection::TransformPythonParamList(move(params))};
	rel->Insert(values);
}

void GuinsooDBPyRelation::Create(const string &table) {
	rel->Create(table);
}

unique_ptr<GuinsooDBPyRelation> GuinsooDBPyRelation::Map(py::function fun) {
	vector<Value> params;
	params.emplace_back(Value::POINTER((uintptr_t)fun.ptr()));
	auto res = make_unique<GuinsooDBPyRelation>(rel->TableFunction("python_map_function", params));
	res->map_function = fun;
	return res;
}

string GuinsooDBPyRelation::Print() {
	std::string rel_res_string;
	{
		py::gil_scoped_release release;
		rel_res_string = rel->Limit(10)->Execute()->ToString();
	}

	return rel->ToString() + "\n---------------------\n-- Result Preview  --\n---------------------\n" +
	       rel_res_string + "\n";
}

py::object GuinsooDBPyRelation::Getattr(const py::str &key) {
	auto key_s = key.cast<string>();
	if (key_s == "alias") {
		return py::str(string(rel->GetAlias()));
	} else if (key_s == "type") {
		return py::str(RelationTypeToString(rel->type));
	} else if (key_s == "columns") {
		py::list res;
		for (auto &col : rel->Columns()) {
			res.append(col.name);
		}
		return move(res);
	} else if (key_s == "types" || key_s == "dtypes") {
		py::list res;
		for (auto &col : rel->Columns()) {
			res.append(col.type.ToString());
		}
		return move(res);
	}
	return py::none();
}

} // namespace guinsoodb

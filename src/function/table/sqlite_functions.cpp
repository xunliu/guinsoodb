#include "guinsoodb/function/table/sqlite_functions.hpp"
#include "guinsoodb/parser/parsed_data/create_view_info.hpp"
#include "guinsoodb/parser/query_node/select_node.hpp"
#include "guinsoodb/parser/expression/star_expression.hpp"
#include "guinsoodb/parser/tableref/table_function_ref.hpp"
#include "guinsoodb/parser/expression/function_expression.hpp"
#include "guinsoodb/catalog/catalog.hpp"

namespace guinsoodb {

void BuiltinFunctions::RegisterSQLiteFunctions() {
	PragmaVersion::RegisterFunction(*this);
	PragmaFunctionPragma::RegisterFunction(*this);
	PragmaCollations::RegisterFunction(*this);
	PragmaTableInfo::RegisterFunction(*this);
	SQLiteMaster::RegisterFunction(*this);
	PragmaDatabaseSize::RegisterFunction(*this);
	PragmaDatabaseList::RegisterFunction(*this);
	PragmaLastProfilingOutput::RegisterFunction(*this);
	PragmaDetailedProfilingOutput::RegisterFunction(*this);

	// CreateViewInfo info;
	// info.schema = DEFAULT_SCHEMA;
	// info.view_name = "sqlite_master";
	// info.on_conflict = OnCreateConflict::REPLACE;

	// auto select = make_unique<SelectNode>();
	// select->select_list.push_back(make_unique<StarExpression>());
	// vector<unique_ptr<ParsedExpression>> children;

	// auto function = make_unique<FunctionExpression>(DEFAULT_SCHEMA, "sqlite_master", children);
	// auto function_expr = make_unique<TableFunctionRef>();
	// function_expr->function = move(function);
	// select->from_table = move(function_expr);
	// info.query = move(select);
	//	catalog.CreateView(transaction, &info);
}

} // namespace guinsoodb

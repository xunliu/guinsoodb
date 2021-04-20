//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/function/table/table_scan.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/function/table_function.hpp"
#include <atomic>

namespace guinsoodb {
class TableCatalogEntry;

struct TableScanBindData : public FunctionData {
	explicit TableScanBindData(TableCatalogEntry *table) : table(table), is_index_scan(false), chunk_count(0) {
	}

	//! The table to scan
	TableCatalogEntry *table;

	//! Whether or not the table scan is an index scan
	bool is_index_scan;
	//! The row ids to fetch (in case of an index scan)
	vector<row_t> result_ids;

	//! How many chunks we already scanned
	std::atomic<idx_t> chunk_count;

	unique_ptr<FunctionData> Copy() override {
		auto result = make_unique<TableScanBindData>(table);
		result->is_index_scan = is_index_scan;
		result->result_ids = result_ids;
		return move(result);
	}
};

//! The table scan function represents a sequential scan over one of GuinsooDB's base tables.
struct TableScanFunction {
	static TableFunction GetFunction();
};

} // namespace guinsoodb

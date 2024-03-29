#include "guinsoodb/function/table/sqlite_functions.hpp"

#include "guinsoodb/catalog/catalog.hpp"
#include "guinsoodb/catalog/catalog_entry/collate_catalog_entry.hpp"
#include "guinsoodb/catalog/catalog_entry/schema_catalog_entry.hpp"
#include "guinsoodb/common/exception.hpp"

namespace guinsoodb {

struct PragmaCollateData : public FunctionOperatorData {
	PragmaCollateData() : offset(0) {
	}

	vector<string> entries;
	idx_t offset;
};

static unique_ptr<FunctionData> PragmaCollateBind(ClientContext &context, vector<Value> &inputs,
                                                  unordered_map<string, Value> &named_parameters,
                                                  vector<LogicalType> &input_table_types,
                                                  vector<string> &input_table_names, vector<LogicalType> &return_types,
                                                  vector<string> &names) {
	names.emplace_back("collname");
	return_types.push_back(LogicalType::VARCHAR);

	return nullptr;
}

unique_ptr<FunctionOperatorData> PragmaCollateInit(ClientContext &context, const FunctionData *bind_data,
                                                   vector<column_t> &column_ids, TableFilterCollection *filters) {
	auto result = make_unique<PragmaCollateData>();

	Catalog::GetCatalog(context).schemas->Scan(context, [&](CatalogEntry *entry) {
		auto schema = (SchemaCatalogEntry *)entry;
		schema->Scan(context, CatalogType::COLLATION_ENTRY,
		             [&](CatalogEntry *entry) { result->entries.push_back(entry->name); });
	});

	return move(result);
}

static void PragmaCollateFunction(ClientContext &context, const FunctionData *bind_data,
                                  FunctionOperatorData *operator_state, DataChunk *input, DataChunk &output) {
	auto &data = (PragmaCollateData &)*operator_state;
	if (data.offset >= data.entries.size()) {
		// finished returning values
		return;
	}
	idx_t next = MinValue<idx_t>(data.offset + STANDARD_VECTOR_SIZE, data.entries.size());
	output.SetCardinality(next - data.offset);
	for (idx_t i = data.offset; i < next; i++) {
		auto index = i - data.offset;
		output.SetValue(0, index, Value(data.entries[i]));
	}

	data.offset = next;
}

void PragmaCollations::RegisterFunction(BuiltinFunctions &set) {
	set.AddFunction(
	    TableFunction("pragma_collations", {}, PragmaCollateFunction, PragmaCollateBind, PragmaCollateInit));
}

} // namespace guinsoodb

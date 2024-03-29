#include "guinsoodb/function/table/sqlite_functions.hpp"
#include "guinsoodb/main/database.hpp"

namespace guinsoodb {

struct PragmaVersionData : public FunctionOperatorData {
	PragmaVersionData() : finished(false) {
	}
	bool finished;
};

static unique_ptr<FunctionData> PragmaVersionBind(ClientContext &context, vector<Value> &inputs,
                                                  unordered_map<string, Value> &named_parameters,
                                                  vector<LogicalType> &input_table_types,
                                                  vector<string> &input_table_names, vector<LogicalType> &return_types,
                                                  vector<string> &names) {
	names.emplace_back("library_version");
	return_types.push_back(LogicalType::VARCHAR);
	names.emplace_back("source_id");
	return_types.push_back(LogicalType::VARCHAR);
	return nullptr;
}

static unique_ptr<FunctionOperatorData> PragmaVersionInit(ClientContext &context, const FunctionData *bind_data,
                                                          vector<column_t> &column_ids,
                                                          TableFilterCollection *filters) {
	return make_unique<PragmaVersionData>();
}

static void PragmaVersionFunction(ClientContext &context, const FunctionData *bind_data,
                                  FunctionOperatorData *operator_state, DataChunk *input, DataChunk &output) {
	auto &data = (PragmaVersionData &)*operator_state;
	if (data.finished) {
		// finished returning values
		return;
	}
	output.SetCardinality(1);
	output.SetValue(0, 0, GuinsooDB::LibraryVersion());
	output.SetValue(1, 0, GuinsooDB::SourceID());
	data.finished = true;
}

void PragmaVersion::RegisterFunction(BuiltinFunctions &set) {
	set.AddFunction(TableFunction("pragma_version", {}, PragmaVersionFunction, PragmaVersionBind, PragmaVersionInit));
}

const char *GuinsooDB::SourceID() {
	return GUINSOODB_SOURCE_ID;
}

const char *GuinsooDB::LibraryVersion() {
	return GUINSOODB_VERSION;
}

} // namespace guinsoodb

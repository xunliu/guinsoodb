//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/parsed_data/create_view_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_data/create_info.hpp"
#include "guinsoodb/parser/statement/select_statement.hpp"

namespace guinsoodb {

struct CreateViewInfo : public CreateInfo {
	CreateViewInfo() : CreateInfo(CatalogType::VIEW_ENTRY) {
	}
	CreateViewInfo(string schema, string view_name)
	    : CreateInfo(CatalogType::VIEW_ENTRY, schema), view_name(view_name) {
	}

	//! Table name to insert to
	string view_name;
	//! Aliases of the view
	vector<string> aliases;
	//! Return types
	vector<LogicalType> types;
	//! The SelectStatement of the view
	unique_ptr<SelectStatement> query;

public:
	unique_ptr<CreateInfo> Copy() const override {
		auto result = make_unique<CreateViewInfo>(schema, view_name);
		CopyProperties(*result);
		result->aliases = aliases;
		result->types = types;
		result->query = unique_ptr_cast<SQLStatement, SelectStatement>(query->Copy());
		return move(result);
	}
};

} // namespace guinsoodb

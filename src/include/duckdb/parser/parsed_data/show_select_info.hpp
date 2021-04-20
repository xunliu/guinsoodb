//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/parsed_data/show_select_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_data/parse_info.hpp"
#include "guinsoodb/parser/query_node.hpp"

namespace guinsoodb {

struct ShowSelectInfo : public ParseInfo {
	//! Types of projected columns
	vector<LogicalType> types;
	//! The QueryNode of select query
	unique_ptr<QueryNode> query;
	//! Aliases of projected columns
	vector<string> aliases;

	unique_ptr<ShowSelectInfo> Copy() {
		auto result = make_unique<ShowSelectInfo>();
		result->types = types;
		result->query = query->Copy();
		result->aliases = aliases;
		return result;
	}
};

} // namespace guinsoodb

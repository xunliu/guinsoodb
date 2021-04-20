//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/parsed_data/pragma_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_data/parse_info.hpp"
#include "guinsoodb/common/types/value.hpp"
#include "guinsoodb/common/unordered_map.hpp"
#include "guinsoodb/parser/parsed_expression.hpp"

namespace guinsoodb {

enum class PragmaType : uint8_t { PRAGMA_STATEMENT, PRAGMA_ASSIGNMENT, PRAGMA_CALL };

struct PragmaInfo : public ParseInfo {
	//! Name of the PRAGMA statement
	string name;
	//! Parameter list (if any)
	vector<Value> parameters;
	//! Named parameter list (if any)
	unordered_map<string, Value> named_parameters;

public:
	unique_ptr<PragmaInfo> Copy() const {
		auto result = make_unique<PragmaInfo>();
		result->name = name;
		result->parameters = parameters;
		result->named_parameters = named_parameters;
		return result;
	}
};

} // namespace guinsoodb

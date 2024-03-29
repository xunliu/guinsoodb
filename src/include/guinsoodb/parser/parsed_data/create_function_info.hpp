//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/parsed_data/create_function_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_data/create_info.hpp"
#include "guinsoodb/function/function.hpp"

namespace guinsoodb {

struct CreateFunctionInfo : public CreateInfo {
	explicit CreateFunctionInfo(CatalogType type) : CreateInfo(type) {
		D_ASSERT(type == CatalogType::SCALAR_FUNCTION_ENTRY || type == CatalogType::AGGREGATE_FUNCTION_ENTRY ||
		         type == CatalogType::TABLE_FUNCTION_ENTRY || type == CatalogType::PRAGMA_FUNCTION_ENTRY ||
		         type == CatalogType::MACRO_ENTRY);
	}

	//! Function name
	string name;
};

} // namespace guinsoodb

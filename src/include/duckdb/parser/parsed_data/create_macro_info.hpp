//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/parsed_data/create_macro_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_data/create_function_info.hpp"
#include "guinsoodb/function/macro_function.hpp"

namespace guinsoodb {

struct CreateMacroInfo : public CreateFunctionInfo {
	CreateMacroInfo() : CreateFunctionInfo(CatalogType::MACRO_ENTRY) {
	}

	unique_ptr<MacroFunction> function;

public:
	unique_ptr<CreateInfo> Copy() const override {
		auto result = make_unique<CreateMacroInfo>();
		result->function = function->Copy();
		result->name = name;
		CopyProperties(*result);
		return move(result);
	}
};

} // namespace guinsoodb

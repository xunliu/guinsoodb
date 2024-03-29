//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/parsed_data/create_index_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_data/create_info.hpp"
#include "guinsoodb/common/enums/index_type.hpp"
#include "guinsoodb/common/vector.hpp"
#include "guinsoodb/parser/tableref/basetableref.hpp"
#include "guinsoodb/parser/parsed_expression.hpp"

namespace guinsoodb {

struct CreateIndexInfo : public CreateInfo {
	CreateIndexInfo() : CreateInfo(CatalogType::INDEX_ENTRY) {
	}

	//! Index Type (e.g., B+-tree, Skip-List, ...)
	IndexType index_type;
	//! Name of the Index
	string index_name;
	//! If it is an unique index
	bool unique = false;
	//! The table to create the index on
	unique_ptr<BaseTableRef> table;
	//! Set of expressions to index by
	vector<unique_ptr<ParsedExpression>> expressions;

public:
	unique_ptr<CreateInfo> Copy() const override {
		auto result = make_unique<CreateIndexInfo>();
		CopyProperties(*result);
		result->index_type = index_type;
		result->index_name = index_name;
		result->unique = unique;
		result->table = unique_ptr_cast<TableRef, BaseTableRef>(table->Copy());
		for (auto &expr : expressions) {
			result->expressions.push_back(expr->Copy());
		}
		return move(result);
	}
};

} // namespace guinsoodb

//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/tableref/table_function_ref.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_expression.hpp"
#include "guinsoodb/parser/tableref.hpp"
#include "guinsoodb/common/vector.hpp"
#include "guinsoodb/parser/statement/select_statement.hpp"

namespace guinsoodb {
//! Represents a Table producing function
class TableFunctionRef : public TableRef {
public:
	TableFunctionRef() : TableRef(TableReferenceType::TABLE_FUNCTION) {
	}

	unique_ptr<ParsedExpression> function;
	vector<string> column_name_alias;

	// if the function takes a subquery as argument its in here
	unique_ptr<SelectStatement> subquery;

public:
	string ToString() const override {
		return function->ToString();
	}

	bool Equals(const TableRef *other_p) const override;

	unique_ptr<TableRef> Copy() override;

	//! Serializes a blob into a BaseTableRef
	void Serialize(Serializer &serializer) override;
	//! Deserializes a blob back into a BaseTableRef
	static unique_ptr<TableRef> Deserialize(Deserializer &source);
};
} // namespace guinsoodb

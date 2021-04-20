//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/column_definition.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/common.hpp"
#include "guinsoodb/common/types/value.hpp"
#include "guinsoodb/parser/parsed_expression.hpp"

namespace guinsoodb {

//! A column of a table.
class ColumnDefinition {
public:
	ColumnDefinition(string name, LogicalType type) : name(name), type(type) {
	}
	ColumnDefinition(string name, LogicalType type, unique_ptr<ParsedExpression> default_value)
	    : name(name), type(type), default_value(move(default_value)) {
	}

	//! The name of the entry
	string name;
	//! The index of the column in the table
	idx_t oid;
	//! The type of the column
	LogicalType type;
	//! The default value of the column (if any)
	unique_ptr<ParsedExpression> default_value;

public:
	ColumnDefinition Copy() const;

	void Serialize(Serializer &serializer) const;
	static ColumnDefinition Deserialize(Deserializer &source);
};

} // namespace guinsoodb
//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/tableref/joinref.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/enums/join_type.hpp"
#include "guinsoodb/common/unordered_set.hpp"
#include "guinsoodb/parser/parsed_expression.hpp"
#include "guinsoodb/parser/tableref.hpp"
#include "guinsoodb/common/vector.hpp"

namespace guinsoodb {
//! Represents a JOIN between two expressions
class JoinRef : public TableRef {
public:
	JoinRef() : TableRef(TableReferenceType::JOIN), is_natural(false) {
	}

	//! The left hand side of the join
	unique_ptr<TableRef> left;
	//! The right hand side of the join
	unique_ptr<TableRef> right;
	//! The join condition
	unique_ptr<ParsedExpression> condition;
	//! The join type
	JoinType type;
	//! Natural join
	bool is_natural;
	//! The set of USING columns (if any)
	vector<string> using_columns;

public:
	bool Equals(const TableRef *other_p) const override;

	unique_ptr<TableRef> Copy() override;

	//! Serializes a blob into a JoinRef
	void Serialize(Serializer &serializer) override;
	//! Deserializes a blob back into a JoinRef
	static unique_ptr<TableRef> Deserialize(Deserializer &source);
};
} // namespace guinsoodb

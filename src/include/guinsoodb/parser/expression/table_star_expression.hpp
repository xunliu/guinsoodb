//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/expression/table_star_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_expression.hpp"

namespace guinsoodb {

//! Represents a table.* expression in the SELECT clause
class TableStarExpression : public ParsedExpression {
public:
	explicit TableStarExpression(string relation_name);

public:
	string ToString() const override;

	static bool Equals(const TableStarExpression *a, const TableStarExpression *b);

	unique_ptr<ParsedExpression> Copy() const override;

	void Serialize(Serializer &serializer) override;
	static unique_ptr<ParsedExpression> Deserialize(ExpressionType type, Deserializer &source);

	string relation_name;
};
} // namespace guinsoodb

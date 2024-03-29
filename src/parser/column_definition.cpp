#include "guinsoodb/parser/column_definition.hpp"
#include "guinsoodb/common/serializer.hpp"

namespace guinsoodb {

ColumnDefinition ColumnDefinition::Copy() const {
	ColumnDefinition copy(name, type);
	copy.oid = oid;
	copy.default_value = default_value ? default_value->Copy() : nullptr;
	return copy;
}

void ColumnDefinition::Serialize(Serializer &serializer) const {
	serializer.WriteString(name);
	type.Serialize(serializer);
	serializer.WriteOptional(default_value);
}

ColumnDefinition ColumnDefinition::Deserialize(Deserializer &source) {
	auto column_name = source.Read<string>();
	auto column_type = LogicalType::Deserialize(source);
	auto default_value = source.ReadOptional<ParsedExpression>();
	return ColumnDefinition(column_name, column_type, move(default_value));
}

} // namespace guinsoodb

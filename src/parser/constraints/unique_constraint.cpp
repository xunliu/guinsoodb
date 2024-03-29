#include "guinsoodb/parser/constraints/unique_constraint.hpp"

#include "guinsoodb/common/serializer.hpp"
#include "guinsoodb/common/limits.hpp"
#include "guinsoodb/parser/keyword_helper.hpp"

namespace guinsoodb {

string UniqueConstraint::ToString() const {
	string base = is_primary_key ? "PRIMARY KEY(" : "UNIQUE(";
	for (idx_t i = 0; i < columns.size(); i++) {
		if (i > 0) {
			base += ", ";
		}
		base += KeywordHelper::WriteOptionallyQuoted(columns[i]);
	}
	return base + ")";
}

unique_ptr<Constraint> UniqueConstraint::Copy() {
	if (index == INVALID_INDEX) {
		return make_unique<UniqueConstraint>(columns, is_primary_key);
	} else {
		D_ASSERT(columns.size() == 0);
		return make_unique<UniqueConstraint>(index, is_primary_key);
	}
}

void UniqueConstraint::Serialize(Serializer &serializer) {
	Constraint::Serialize(serializer);
	serializer.Write<bool>(is_primary_key);
	serializer.Write<uint64_t>(index);
	D_ASSERT(columns.size() <= NumericLimits<uint32_t>::Maximum());
	serializer.Write<uint32_t>((uint32_t)columns.size());
	for (auto &column : columns) {
		serializer.WriteString(column);
	}
}

unique_ptr<Constraint> UniqueConstraint::Deserialize(Deserializer &source) {
	auto is_primary_key = source.Read<bool>();
	auto index = source.Read<uint64_t>();
	auto column_count = source.Read<uint32_t>();

	if (index != INVALID_INDEX) {
		// single column parsed constraint
		return make_unique<UniqueConstraint>(index, is_primary_key);
	} else {
		// column list parsed constraint
		vector<string> columns;
		for (uint32_t i = 0; i < column_count; i++) {
			auto column_name = source.Read<string>();
			columns.push_back(column_name);
		}
		return make_unique<UniqueConstraint>(columns, is_primary_key);
	}
}

} // namespace guinsoodb

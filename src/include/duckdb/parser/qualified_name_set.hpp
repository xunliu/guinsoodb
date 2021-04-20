//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/qualified_name_set.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/qualified_name.hpp"
#include "guinsoodb/common/types/hash.hpp"
#include "guinsoodb/common/unordered_set.hpp"

namespace guinsoodb {

struct QualifiedColumnHashFunction {
	uint64_t operator()(const QualifiedColumnName &a) const {
		std::hash<std::string> str_hasher;
		return str_hasher(a.schema) ^ str_hasher(a.table) ^ str_hasher(a.column);
	}
};

struct QualifiedColumnEquality {
	bool operator()(const QualifiedColumnName &a, const QualifiedColumnName &b) const {
		return a.schema == b.schema && a.table == b.table && a.column == b.column;
	}
};

using qualified_column_set_t = unordered_set<QualifiedColumnName, QualifiedColumnHashFunction, QualifiedColumnEquality>;

} // namespace guinsoodb

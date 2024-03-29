//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/tableref/emptytableref.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/tableref.hpp"

namespace guinsoodb {
//! Represents a cross product
class EmptyTableRef : public TableRef {
public:
	EmptyTableRef() : TableRef(TableReferenceType::EMPTY) {
	}

public:
	bool Equals(const TableRef *other_p) const override;

	unique_ptr<TableRef> Copy() override;

	//! Serializes a blob into a DummyTableRef
	void Serialize(Serializer &serializer) override;
	//! Deserializes a blob back into a DummyTableRef
	static unique_ptr<TableRef> Deserialize(Deserializer &source);
};
} // namespace guinsoodb

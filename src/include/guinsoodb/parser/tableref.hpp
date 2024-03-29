//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/tableref.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/common.hpp"
#include "guinsoodb/common/enums/tableref_type.hpp"
#include "guinsoodb/parser/parsed_data/sample_options.hpp"

namespace guinsoodb {
class Deserializer;
class Serializer;

//! Represents a generic expression that returns a table.
class TableRef {
public:
	explicit TableRef(TableReferenceType type) : type(type) {
	}
	virtual ~TableRef() {
	}

	TableReferenceType type;
	string alias;
	//! Sample options (if any)
	unique_ptr<SampleOptions> sample;
	//! The location in the query (if any)
	idx_t query_location = INVALID_INDEX;

public:
	//! Convert the object to a string
	virtual string ToString() const {
		return string();
	}
	void Print();

	virtual bool Equals(const TableRef *other) const;

	virtual unique_ptr<TableRef> Copy() = 0;

	//! Serializes a TableRef to a stand-alone binary blob
	virtual void Serialize(Serializer &serializer);
	//! Deserializes a blob back into a TableRef
	static unique_ptr<TableRef> Deserialize(Deserializer &source);

	//! Copy the properties of this table ref to the target
	void CopyProperties(TableRef &target);
};
} // namespace guinsoodb

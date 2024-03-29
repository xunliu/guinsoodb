#include "guinsoodb/parser/tableref/emptytableref.hpp"

#include "guinsoodb/common/serializer.hpp"

namespace guinsoodb {

bool EmptyTableRef::Equals(const TableRef *other) const {
	return TableRef::Equals(other);
}

unique_ptr<TableRef> EmptyTableRef::Copy() {
	return make_unique<EmptyTableRef>();
}

void EmptyTableRef::Serialize(Serializer &serializer) {
	TableRef::Serialize(serializer);
}

unique_ptr<TableRef> EmptyTableRef::Deserialize(Deserializer &source) {
	return make_unique<EmptyTableRef>();
}

} // namespace guinsoodb

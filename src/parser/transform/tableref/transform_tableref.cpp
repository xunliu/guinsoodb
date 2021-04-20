#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/parser/tableref.hpp"
#include "guinsoodb/parser/transformer.hpp"

namespace guinsoodb {

unique_ptr<TableRef> Transformer::TransformTableRefNode(guinsoodb_libpgquery::PGNode *n) {
	switch (n->type) {
	case guinsoodb_libpgquery::T_PGRangeVar:
		return TransformRangeVar(reinterpret_cast<guinsoodb_libpgquery::PGRangeVar *>(n));
	case guinsoodb_libpgquery::T_PGJoinExpr:
		return TransformJoin(reinterpret_cast<guinsoodb_libpgquery::PGJoinExpr *>(n));
	case guinsoodb_libpgquery::T_PGRangeSubselect:
		return TransformRangeSubselect(reinterpret_cast<guinsoodb_libpgquery::PGRangeSubselect *>(n));
	case guinsoodb_libpgquery::T_PGRangeFunction:
		return TransformRangeFunction(reinterpret_cast<guinsoodb_libpgquery::PGRangeFunction *>(n));
	default:
		throw NotImplementedException("From Type %d not supported yet...", n->type);
	}
}

} // namespace guinsoodb

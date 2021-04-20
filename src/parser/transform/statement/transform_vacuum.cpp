#include "guinsoodb/parser/statement/vacuum_statement.hpp"
#include "guinsoodb/parser/transformer.hpp"

namespace guinsoodb {

unique_ptr<VacuumStatement> Transformer::TransformVacuum(guinsoodb_libpgquery::PGNode *node) {
	auto stmt = reinterpret_cast<guinsoodb_libpgquery::PGVacuumStmt *>(node);
	D_ASSERT(stmt);
	(void)stmt;
	auto result = make_unique<VacuumStatement>();
	return result;
}

} // namespace guinsoodb

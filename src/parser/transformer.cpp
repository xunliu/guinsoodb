#include "guinsoodb/parser/transformer.hpp"

#include "guinsoodb/parser/expression/list.hpp"
#include "guinsoodb/parser/statement/list.hpp"
#include "guinsoodb/parser/tableref/emptytableref.hpp"

namespace guinsoodb {

bool Transformer::TransformParseTree(guinsoodb_libpgquery::PGList *tree, vector<unique_ptr<SQLStatement>> &statements) {
	for (auto entry = tree->head; entry != nullptr; entry = entry->next) {
		SetParamCount(0);
		auto stmt = TransformStatement((guinsoodb_libpgquery::PGNode *)entry->data.ptr_value);
		if (!stmt) {
			statements.clear();
			return false;
		}
		stmt->n_param = ParamCount();
		statements.push_back(move(stmt));
	}
	return true;
}

unique_ptr<SQLStatement> Transformer::TransformStatement(guinsoodb_libpgquery::PGNode *stmt) {
	switch (stmt->type) {
	case guinsoodb_libpgquery::T_PGRawStmt: {
		auto raw_stmt = (guinsoodb_libpgquery::PGRawStmt *)stmt;
		auto result = TransformStatement(raw_stmt->stmt);
		if (result) {
			result->stmt_location = raw_stmt->stmt_location;
			result->stmt_length = raw_stmt->stmt_len;
		}
		return result;
	}
	case guinsoodb_libpgquery::T_PGSelectStmt:
		return TransformSelect(stmt);
	case guinsoodb_libpgquery::T_PGCreateStmt:
		return TransformCreateTable(stmt);
	case guinsoodb_libpgquery::T_PGCreateSchemaStmt:
		return TransformCreateSchema(stmt);
	case guinsoodb_libpgquery::T_PGViewStmt:
		return TransformCreateView(stmt);
	case guinsoodb_libpgquery::T_PGCreateSeqStmt:
		return TransformCreateSequence(stmt);
	case guinsoodb_libpgquery::T_PGCreateFunctionStmt:
		return TransformCreateFunction(stmt);
	case guinsoodb_libpgquery::T_PGDropStmt:
		return TransformDrop(stmt);
	case guinsoodb_libpgquery::T_PGInsertStmt:
		return TransformInsert(stmt);
	case guinsoodb_libpgquery::T_PGCopyStmt:
		return TransformCopy(stmt);
	case guinsoodb_libpgquery::T_PGTransactionStmt:
		return TransformTransaction(stmt);
	case guinsoodb_libpgquery::T_PGDeleteStmt:
		return TransformDelete(stmt);
	case guinsoodb_libpgquery::T_PGUpdateStmt:
		return TransformUpdate(stmt);
	case guinsoodb_libpgquery::T_PGIndexStmt:
		return TransformCreateIndex(stmt);
	case guinsoodb_libpgquery::T_PGAlterTableStmt:
		return TransformAlter(stmt);
	case guinsoodb_libpgquery::T_PGRenameStmt:
		return TransformRename(stmt);
	case guinsoodb_libpgquery::T_PGPrepareStmt:
		return TransformPrepare(stmt);
	case guinsoodb_libpgquery::T_PGExecuteStmt:
		return TransformExecute(stmt);
	case guinsoodb_libpgquery::T_PGDeallocateStmt:
		return TransformDeallocate(stmt);
	case guinsoodb_libpgquery::T_PGCreateTableAsStmt:
		return TransformCreateTableAs(stmt);
	case guinsoodb_libpgquery::T_PGPragmaStmt:
		return TransformPragma(stmt);
	case guinsoodb_libpgquery::T_PGExportStmt:
		return TransformExport(stmt);
	case guinsoodb_libpgquery::T_PGImportStmt:
		return TransformImport(stmt);
	case guinsoodb_libpgquery::T_PGExplainStmt:
		return TransformExplain(stmt);
	case guinsoodb_libpgquery::T_PGVacuumStmt:
		return TransformVacuum(stmt);
	case guinsoodb_libpgquery::T_PGVariableShowStmt:
		return TransformShow(stmt);
	case guinsoodb_libpgquery::T_PGVariableShowSelectStmt:
		return TransformShowSelect(stmt);
	case guinsoodb_libpgquery::T_PGCallStmt:
		return TransformCall(stmt);
	case guinsoodb_libpgquery::T_PGVariableSetStmt:
		return TransformSet(stmt);
	case guinsoodb_libpgquery::T_PGCheckPointStmt:
		return TransformCheckpoint(stmt);
	case guinsoodb_libpgquery::T_PGLoadStmt:
		return TransformLoad(stmt);
	default:
		throw NotImplementedException(NodetypeToString(stmt->type));
	}
	return nullptr;
}

} // namespace guinsoodb

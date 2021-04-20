#include "guinsoodb/parser/statement/export_statement.hpp"
#include "guinsoodb/parser/transformer.hpp"

namespace guinsoodb {

unique_ptr<ExportStatement> Transformer::TransformExport(guinsoodb_libpgquery::PGNode *node) {
	auto stmt = reinterpret_cast<guinsoodb_libpgquery::PGExportStmt *>(node);
	auto info = make_unique<CopyInfo>();
	info->file_path = stmt->filename;
	info->format = "csv";
	info->is_from = false;
	// handle export options
	TransformCopyOptions(*info, stmt->options);

	return make_unique<ExportStatement>(move(info));
}

} // namespace guinsoodb

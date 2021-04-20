#include "guinsoodb/parser/statement/create_statement.hpp"
#include "guinsoodb/parser/parsed_data/create_sequence_info.hpp"
#include "guinsoodb/parser/transformer.hpp"
#include "guinsoodb/common/operator/cast_operators.hpp"

namespace guinsoodb {

unique_ptr<CreateStatement> Transformer::TransformCreateSequence(guinsoodb_libpgquery::PGNode *node) {
	auto stmt = reinterpret_cast<guinsoodb_libpgquery::PGCreateSeqStmt *>(node);

	auto result = make_unique<CreateStatement>();
	auto info = make_unique<CreateSequenceInfo>();

	auto qname = TransformQualifiedName(stmt->sequence);
	info->schema = qname.schema;
	info->name = qname.name;

	if (stmt->options) {
		guinsoodb_libpgquery::PGListCell *cell = nullptr;
		for_each_cell(cell, stmt->options->head) {
			auto *def_elem = reinterpret_cast<guinsoodb_libpgquery::PGDefElem *>(cell->data.ptr_value);
			string opt_name = string(def_elem->defname);

			auto val = (guinsoodb_libpgquery::PGValue *)def_elem->arg;
			if (def_elem->defaction == guinsoodb_libpgquery::PG_DEFELEM_UNSPEC && !val) { // e.g. NO MINVALUE
				continue;
			}
			D_ASSERT(val);
			int64_t opt_value;
			if (val->type == guinsoodb_libpgquery::T_PGInteger) {
				opt_value = val->val.ival;
			} else if (val->type == guinsoodb_libpgquery::T_PGFloat) {
				if (!TryCast::Operation<string_t, int64_t>(string_t(val->val.str), opt_value, true)) {
					throw ParserException("Expected an integer argument for option %s", opt_name);
				}
			} else {
				throw ParserException("Expected an integer argument for option %s", opt_name);
			}
			if (opt_name == "increment") {
				info->increment = opt_value;
				if (info->increment == 0) {
					throw ParserException("Increment must not be zero");
				}
				if (info->increment < 0) {
					info->start_value = info->max_value = -1;
					info->min_value = NumericLimits<int64_t>::Minimum();
				} else {
					info->start_value = info->min_value = 1;
					info->max_value = NumericLimits<int64_t>::Maximum();
				}
			} else if (opt_name == "minvalue") {
				info->min_value = opt_value;
				if (info->increment > 0) {
					info->start_value = info->min_value;
				}
			} else if (opt_name == "maxvalue") {
				info->max_value = opt_value;
				if (info->increment < 0) {
					info->start_value = info->max_value;
				}
			} else if (opt_name == "start") {
				info->start_value = opt_value;
			} else if (opt_name == "cycle") {
				info->cycle = opt_value > 0;
			} else {
				throw ParserException("Unrecognized option \"%s\" for CREATE SEQUENCE", opt_name);
			}
		}
	}
	info->temporary = !stmt->sequence->relpersistence;
	info->on_conflict =
	    stmt->if_not_exists ? OnCreateConflict::IGNORE_ON_CONFLICT : OnCreateConflict::ERROR_ON_CONFLICT;
	if (info->max_value <= info->min_value) {
		throw ParserException("MINVALUE (%lld) must be less than MAXVALUE (%lld)", info->min_value, info->max_value);
	}
	if (info->start_value < info->min_value) {
		throw ParserException("START value (%lld) cannot be less than MINVALUE (%lld)", info->start_value,
		                      info->min_value);
	}
	if (info->start_value > info->max_value) {
		throw ParserException("START value (%lld) cannot be greater than MAXVALUE (%lld)", info->start_value,
		                      info->max_value);
	}
	result->info = move(info);
	return result;
}

} // namespace guinsoodb

#include "fts-extension.hpp"
#include "fts_indexing.hpp"
#include "libstemmer.h"

#include "guinsoodb.hpp"
#include "guinsoodb/common/exception.hpp"
#include "guinsoodb/common/string_util.hpp"
#include "guinsoodb/function/scalar_function.hpp"
#include "guinsoodb/parser/parsed_data/create_scalar_function_info.hpp"
#include "guinsoodb/parser/parsed_data/create_pragma_function_info.hpp"

#include "guinsoodb/main/client_context.hpp"
#include "guinsoodb/catalog/catalog.hpp"

namespace guinsoodb {

static void stem_function(DataChunk &args, ExpressionState &state, Vector &result) {
	auto &input_vector = args.data[0];
	auto &stemmer_vector = args.data[1];

	BinaryExecutor::Execute<string_t, string_t, string_t>(
	    input_vector, stemmer_vector, result, args.size(), [&](string_t input, string_t stemmer) {
		    auto input_data = input.GetDataUnsafe();
		    auto input_size = input.GetSize();

		    if (stemmer.GetString() == "none") {
			    auto output = StringVector::AddString(result, input_data, input_size);
			    return output;
		    }

		    struct sb_stemmer *s = sb_stemmer_new(stemmer.GetString().c_str(), "UTF_8");
		    if (s == 0) {
			    const char **stemmers = sb_stemmer_list();
			    size_t n_stemmers = 27;
			    throw Exception(StringUtil::Format(
			        "Unrecognized stemmer '%s'. Supported stemmers are: ['%s'], or use 'none' for no stemming",
			        stemmer.GetString(),
			        StringUtil::Join(stemmers, n_stemmers, "', '", [](const char *st) { return st; })));
		    }

		    auto output_data = (char *)sb_stemmer_stem(s, (const sb_symbol *)input_data, input_size);
		    auto output_size = sb_stemmer_length(s);
		    auto output = StringVector::AddString(result, output_data, output_size);

		    sb_stemmer_delete(s);
		    return output;
	    });
}

void FTSExtension::Load(GuinsooDB &db) {
	ScalarFunction stem_func("stem", {LogicalType::VARCHAR, LogicalType::VARCHAR}, LogicalType::VARCHAR, stem_function);
	CreateScalarFunctionInfo stem_info(stem_func);

	auto create_fts_index_func = PragmaFunction::PragmaCall(
	    "create_fts_index", create_fts_index_query, {LogicalType::VARCHAR, LogicalType::VARCHAR}, LogicalType::VARCHAR);
	create_fts_index_func.named_parameters["stemmer"] = LogicalType::VARCHAR;
	create_fts_index_func.named_parameters["stopwords"] = LogicalType::VARCHAR;
	create_fts_index_func.named_parameters["ignore"] = LogicalType::VARCHAR;
	create_fts_index_func.named_parameters["strip_accents"] = LogicalType::BOOLEAN;
	create_fts_index_func.named_parameters["lower"] = LogicalType::BOOLEAN;
	create_fts_index_func.named_parameters["overwrite"] = LogicalType::BOOLEAN;
	CreatePragmaFunctionInfo create_fts_index_info(create_fts_index_func);

	auto drop_fts_index_func =
	    PragmaFunction::PragmaCall("drop_fts_index", drop_fts_index_query, {LogicalType::VARCHAR});
	CreatePragmaFunctionInfo drop_fts_index_info(drop_fts_index_func);

	Connection conn(db);
	conn.BeginTransaction();
	auto &catalog = Catalog::GetCatalog(*conn.context);
	catalog.CreateFunction(*conn.context, &stem_info);
	catalog.CreatePragmaFunction(*conn.context, &create_fts_index_info);
	catalog.CreatePragmaFunction(*conn.context, &drop_fts_index_info);
	conn.Commit();
}

} // namespace guinsoodb

#include "icu-extension.hpp"
#include "icu-collate.hpp"

#include "guinsoodb/main/database.hpp"
#include "guinsoodb/main/connection.hpp"

#include "guinsoodb/common/string_util.hpp"
#include "guinsoodb/planner/expression/bound_function_expression.hpp"
#include "guinsoodb/function/scalar_function.hpp"
#include "guinsoodb/common/vector_operations/unary_executor.hpp"
#include "guinsoodb/parser/parsed_data/create_collation_info.hpp"
#include "guinsoodb/parser/parsed_data/create_scalar_function_info.hpp"
#include "guinsoodb/execution/expression_executor.hpp"
#include "guinsoodb/catalog/catalog.hpp"

#include <cassert>

namespace guinsoodb {

struct IcuBindData : public FunctionData {
	std::unique_ptr<icu::Collator> collator;
	string language;
	string country;

	IcuBindData(string language_p, string country_p) : language(move(language_p)), country(move(country_p)) {
		UErrorCode status = U_ZERO_ERROR;
		this->collator = std::unique_ptr<icu::Collator>(
		    icu::Collator::createInstance(icu::Locale(language.c_str(), country.c_str()), status));
		if (U_FAILURE(status)) {
			throw Exception("Failed to create ICU collator!");
		}
	}

	unique_ptr<FunctionData> Copy() override {
		return make_unique<IcuBindData>(language.c_str(), country.c_str());
	}
};

static int32_t icu_get_sort_key(icu::Collator &collator, string_t input, unique_ptr<char[]> &buffer,
                                int32_t &buffer_size) {
	int32_t string_size =
	    collator.getSortKey(icu::UnicodeString::fromUTF8(icu::StringPiece(input.GetDataUnsafe(), input.GetSize())),
	                        (uint8_t *)buffer.get(), buffer_size);
	if (string_size > buffer_size) {
		// have to resize the buffer
		buffer_size = string_size;
		buffer = unique_ptr<char[]>(new char[buffer_size]);

		string_size =
		    collator.getSortKey(icu::UnicodeString::fromUTF8(icu::StringPiece(input.GetDataUnsafe(), input.GetSize())),
		                        (uint8_t *)buffer.get(), buffer_size);
	}
	return string_size;
}

static void icu_collate_function(DataChunk &args, ExpressionState &state, Vector &result) {
	const char hexa_table[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	auto &func_expr = (BoundFunctionExpression &)state.expr;
	auto &info = (IcuBindData &)*func_expr.bind_info;
	auto &collator = *info.collator;

	unique_ptr<char[]> buffer;
	int32_t buffer_size = 0;
	UnaryExecutor::Execute<string_t, string_t>(args.data[0], result, args.size(), [&](string_t input) {
		// create a sort key from the string
		int32_t string_size = icu_get_sort_key(collator, input, buffer, buffer_size);
		// convert the sort key to hexadecimal
		auto str_result = StringVector::EmptyString(result, (string_size - 1) * 2);
		auto str_data = str_result.GetDataWriteable();
		for (idx_t i = 0; i < string_size - 1; i++) {
			uint8_t byte = uint8_t(buffer[i]);
			assert(byte != 0);
			str_data[i * 2] = hexa_table[byte / 16];
			str_data[i * 2 + 1] = hexa_table[byte % 16];
		}
		// printf("%s: %s\n", input.GetString().c_str(), str_result.GetString().c_str());
		return str_result;
	});
}

static unique_ptr<FunctionData> icu_collate_bind(ClientContext &context, ScalarFunction &bound_function,
                                                 vector<unique_ptr<Expression>> &arguments) {
	auto splits = StringUtil::Split(bound_function.name, "_");
	if (splits.size() == 1) {
		return make_unique<IcuBindData>(splits[0], "");
	} else if (splits.size() == 2) {
		return make_unique<IcuBindData>(splits[0], splits[1]);
	} else {
		throw InternalException("Expected one or two splits");
	}
}

static unique_ptr<FunctionData> icu_sort_key_bind(ClientContext &context, ScalarFunction &bound_function,
                                                  vector<unique_ptr<Expression>> &arguments) {
	if (!arguments[1]->IsFoldable()) {
		throw NotImplementedException("ICU_SORT_KEY(VARCHAR, VARCHAR) with non-constant collation is not supported");
	}
	Value val = ExpressionExecutor::EvaluateScalar(*arguments[1]).CastAs(LogicalType::VARCHAR);
	if (val.is_null) {
		throw NotImplementedException("ICU_SORT_KEY(VARCHAR, VARCHAR) expected a non-null collation");
	}
	auto splits = StringUtil::Split(val.str_value, "_");
	if (splits.size() == 1) {
		return make_unique<IcuBindData>(splits[0], "");
	} else if (splits.size() == 2) {
		return make_unique<IcuBindData>(splits[0], splits[1]);
	} else {
		throw InternalException("Expected one or two splits");
	}
}

static ScalarFunction get_icu_function(string collation) {
	return ScalarFunction(collation, {LogicalType::VARCHAR}, LogicalType::VARCHAR, icu_collate_function, false,
	                      icu_collate_bind);
}

void ICUExtension::Load(GuinsooDB &db) {
	// load the collations
	Connection con(db);
	con.BeginTransaction();

	auto &catalog = Catalog::GetCatalog(*con.context);

	// iterate over all the collations
	int32_t count;
	auto locales = icu::Collator::getAvailableLocales(count);
	for (int32_t i = 0; i < count; i++) {
		string collation;
		if (string(locales[i].getCountry()).empty()) {
			// language only
			collation = locales[i].getLanguage();
		} else {
			// language + country
			collation = locales[i].getLanguage() + string("_") + locales[i].getCountry();
		}
		collation = StringUtil::Lower(collation);

		CreateCollationInfo info(collation, get_icu_function(collation), false, true);
		info.on_conflict = OnCreateConflict::IGNORE_ON_CONFLICT;
		catalog.CreateCollation(*con.context, &info);
	}
	ScalarFunction sort_key("icu_sort_key", {LogicalType::VARCHAR, LogicalType::VARCHAR}, LogicalType::VARCHAR,
	                        icu_collate_function, false, icu_sort_key_bind);

	CreateScalarFunctionInfo sort_key_info(move(sort_key));
	catalog.CreateFunction(*con.context, &sort_key_info);

	con.Commit();
}

} // namespace guinsoodb

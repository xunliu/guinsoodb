#include "guinsoodb/parser/transformer.hpp"
#include "guinsoodb/parser/parsed_data/sample_options.hpp"
#include "guinsoodb/parser/expression/constant_expression.hpp"
#include "guinsoodb/common/string_util.hpp"

namespace guinsoodb {

static SampleMethod GetSampleMethod(const string &method) {
	auto lmethod = StringUtil::Lower(method);
	if (lmethod == "system") {
		return SampleMethod::SYSTEM_SAMPLE;
	} else if (lmethod == "bernoulli") {
		return SampleMethod::BERNOULLI_SAMPLE;
	} else if (lmethod == "reservoir") {
		return SampleMethod::RESERVOIR_SAMPLE;
	} else {
		throw ParserException("Unrecognized sampling method %s, expected system, bernoulli or reservoir", method);
	}
}

unique_ptr<SampleOptions> Transformer::TransformSampleOptions(guinsoodb_libpgquery::PGNode *options) {
	if (!options) {
		return nullptr;
	}
	auto result = make_unique<SampleOptions>();
	auto &sample_options = (guinsoodb_libpgquery::PGSampleOptions &)*options;
	auto &sample_size = (guinsoodb_libpgquery::PGSampleSize &)*sample_options.sample_size;
	auto sample_value = TransformValue(sample_size.sample_size)->value;
	result->is_percentage = sample_size.is_percentage;
	if (sample_size.is_percentage) {
		// sample size is given in sample_size: use system sampling
		auto percentage = sample_value.GetValue<double>();
		if (percentage < 0 || percentage > 100) {
			throw ParserException("Sample sample_size %llf out of range, must be between 0 and 100", percentage);
		}
		result->sample_size = Value::DOUBLE(percentage);
		result->method = SampleMethod::SYSTEM_SAMPLE;
	} else {
		// sample size is given in rows: use reservoir sampling
		auto rows = sample_value.GetValue<int64_t>();
		if (rows < 0) {
			throw ParserException("Sample rows %lld out of range, must be bigger than or equal to 0", rows);
		}
		result->sample_size = Value::BIGINT(rows);
		result->method = SampleMethod::RESERVOIR_SAMPLE;
	}
	if (sample_options.method) {
		result->method = GetSampleMethod(sample_options.method);
	}
	result->seed = sample_options.seed;
	return result;
}

} // namespace guinsoodb

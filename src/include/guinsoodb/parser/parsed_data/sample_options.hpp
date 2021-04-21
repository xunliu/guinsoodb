//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/parsed_data/sample_options.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/common.hpp"
#include "guinsoodb/parser/parsed_expression.hpp"
#include "guinsoodb/common/vector.hpp"
#include "guinsoodb/common/types/value.hpp"

namespace guinsoodb {

enum class SampleMethod : uint8_t { SYSTEM_SAMPLE = 0, BERNOULLI_SAMPLE = 1, RESERVOIR_SAMPLE = 2 };

string SampleMethodToString(SampleMethod method);

struct SampleOptions {
	Value sample_size;
	bool is_percentage;
	SampleMethod method;
	int64_t seed;

	unique_ptr<SampleOptions> Copy();
	void Serialize(Serializer &serializer);
	static unique_ptr<SampleOptions> Deserialize(Deserializer &source);
	static bool Equals(SampleOptions *a, SampleOptions *b);
};

} // namespace guinsoodb

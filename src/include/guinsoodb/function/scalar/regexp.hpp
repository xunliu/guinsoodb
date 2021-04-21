//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/function/scalar/regexp.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/function/function_set.hpp"
#include "re2/re2.h"

namespace guinsoodb {

struct RegexpMatchesBindData : public FunctionData {
	RegexpMatchesBindData(guinsoodb_re2::RE2::Options options, std::unique_ptr<guinsoodb_re2::RE2> constant_pattern,
	                      string range_min, string range_max, bool range_success);
	~RegexpMatchesBindData() override;

	guinsoodb_re2::RE2::Options options;
	std::unique_ptr<guinsoodb_re2::RE2> constant_pattern;
	string range_min, range_max;
	bool range_success;

	unique_ptr<FunctionData> Copy() override;
};

struct RegexpReplaceBindData : public FunctionData {
	guinsoodb_re2::RE2::Options options;
	bool global_replace;

	unique_ptr<FunctionData> Copy() override;
};

} // namespace guinsoodb

//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/parsed_data/create_sequence_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_data/create_info.hpp"
#include "guinsoodb/common/limits.hpp"

namespace guinsoodb {

struct CreateSequenceInfo : public CreateInfo {
	CreateSequenceInfo()
	    : CreateInfo(CatalogType::SEQUENCE_ENTRY), name(string()), usage_count(0), increment(1), min_value(1),
	      max_value(NumericLimits<int64_t>::Maximum()), start_value(1), cycle(false) {
	}

	//! Sequence name to create
	string name;
	//! Usage count of the sequence
	uint64_t usage_count;
	//! The increment value
	int64_t increment;
	//! The minimum value of the sequence
	int64_t min_value;
	//! The maximum value of the sequence
	int64_t max_value;
	//! The start value of the sequence
	int64_t start_value;
	//! Whether or not the sequence cycles
	bool cycle;

public:
	unique_ptr<CreateInfo> Copy() const override {
		auto result = make_unique<CreateSequenceInfo>();
		CopyProperties(*result);
		result->name = name;
		result->usage_count = usage_count;
		result->increment = increment;
		result->min_value = min_value;
		result->max_value = max_value;
		result->start_value = start_value;
		result->cycle = cycle;
		return move(result);
	}
};

} // namespace guinsoodb

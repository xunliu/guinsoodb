//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/parser/parsed_data/vacuum_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/parser/parsed_data/parse_info.hpp"

namespace guinsoodb {

struct LoadInfo : public ParseInfo {
	std::string filename;

public:
	unique_ptr<LoadInfo> Copy() const {
		auto result = make_unique<LoadInfo>();
		result->filename = filename;
		return result;
	}
};

} // namespace guinsoodb

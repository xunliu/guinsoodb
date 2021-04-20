//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/main/extension.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/common.hpp"
#include "guinsoodb/common/winapi.hpp"

namespace guinsoodb {
class GuinsooDB;

//! The Extension class is the base class used to define extensions
class Extension {
public:
	DUCKDB_API virtual void Load(GuinsooDB &db) = 0;
	DUCKDB_API virtual ~Extension() = default;
};

} // namespace guinsoodb
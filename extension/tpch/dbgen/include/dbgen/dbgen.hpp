//===----------------------------------------------------------------------===//
//
//                         GuinsooDB
//
// dbgen.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb.hpp"
#ifndef DUCKDB_AMALGAMATION
#include "guinsoodb/catalog/catalog.hpp"
#include "guinsoodb/common/types/data_chunk.hpp"
#endif

namespace guinsoodb {
class ClientContext;
}

namespace tpch {

struct DBGenWrapper {
	//! Create the TPC-H tables in the given schema with the given suffix
	static void CreateTPCHSchema(guinsoodb::ClientContext &context, std::string schema, std::string suffix);
	//! Load the TPC-H data of the given scale factor
	static void LoadTPCHData(guinsoodb::ClientContext &context, double sf, std::string schema, std::string suffix);

	//! Gets the specified TPC-H Query number as a string
	static std::string GetQuery(int query);
	//! Returns the CSV answer of a TPC-H query
	static std::string GetAnswer(double sf, int query);
};

} // namespace tpch

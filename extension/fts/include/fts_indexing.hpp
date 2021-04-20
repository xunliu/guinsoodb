//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// fts_indexing.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/main/client_context.hpp"

namespace guinsoodb {

string drop_fts_index_query(ClientContext &context, const FunctionParameters &parameters);
string create_fts_index_query(ClientContext &context, const FunctionParameters &parameters);

} // namespace guinsoodb

#pragma once

#include "guinsoodb/catalog/catalog.hpp"
#include "guinsoodb.hpp"

namespace imdb {
//! Adds the IMDB tables to the database
void dbgen(guinsoodb::GuinsooDB &database);

//! Gets the specified IMDB JOB Query number as a string
std::string get_query(int query);

} // namespace imdb

#include "guinsoodb.hpp"
#include "sqlite3.h"

namespace sqlite {

//! Transfer all data inside the GuinsooDB connection to the given sqlite database
bool TransferDatabase(guinsoodb::Connection &con, sqlite3 *sqlite);

//! Fires a query to a SQLite database, returning a QueryResult object. Interrupt should be initially set to 0. If
//! interrupt becomes 1 at any point query execution is cancelled.
guinsoodb::unique_ptr<guinsoodb::QueryResult> QueryDatabase(sqlite3 *sqlite, std::string query, volatile int &interrupt);

}; // namespace sqlite

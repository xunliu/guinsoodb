#pragma once

#include "guinsoodb/main/appender.hpp"
#include "guinsoodb/main/connection.hpp"

#include <memory>

namespace tpcds {

struct tpcds_table_def {
	const char *name;
	int fl_small;
	int fl_child;
};

#define CALL_CENTER 0
#define DBGEN_VERSION 24

struct tpcds_append_information {
	tpcds_append_information(guinsoodb::GuinsooDB &db, std::string schema_name, std::string table_name) : connection(db), appender(connection, schema_name, table_name)
	     {
	}

	guinsoodb::Connection connection;
	guinsoodb::Appender appender;

	tpcds_table_def table_def;
};

} // namespace tpcds

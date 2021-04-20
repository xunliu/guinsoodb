/// @file
/// @brief schema and dut classes for GuinsooDB

#ifndef GUINSOODB_HH
#define GUINSOODB_HH

#include "guinsoodb.hpp"

#include "dut.hh"
#include "relmodel.hh"
#include "schema.hh"

struct guinsoodb_connection {
	std::unique_ptr<guinsoodb::GuinsooDB> database;
	std::unique_ptr<guinsoodb::Connection> connection;
	char *zErrMsg = 0;
	int rc;
	void q(const char *query);
	guinsoodb_connection(std::string &conninfo);
};

struct schema_guinsoodb : schema, guinsoodb_connection {
	schema_guinsoodb(std::string &conninfo, bool no_catalog);
	virtual std::string quote_name(const std::string &id) {
		return id;
	}
};

struct dut_guinsoodb : dut_base, guinsoodb_connection {
	virtual void test(const std::string &stmt);
	dut_guinsoodb(std::string &conninfo);
};
#endif

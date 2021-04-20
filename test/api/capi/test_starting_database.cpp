#include "catch.hpp"
#include "guinsoodb.h"

using namespace std;

TEST_CASE("Simple In-Memory DB Start Up and Shutdown", "[simplestartup]") {
	guinsoodb_database database;
	guinsoodb_connection connection;

	// open and close a database in in-memory mode
	REQUIRE(guinsoodb_open(NULL, &database) == GuinsooDBSuccess);
	REQUIRE(guinsoodb_connect(database, &connection) == GuinsooDBSuccess);
	guinsoodb_disconnect(&connection);
	guinsoodb_close(&database);
}

TEST_CASE("Multiple In-Memory DB Start Up and Shutdown", "[multiplestartup]") {
	guinsoodb_database database[10];
	guinsoodb_connection connection[100];

	// open and close 10 databases
	// and open 10 connections per database
	for (size_t i = 0; i < 10; i++) {
		REQUIRE(guinsoodb_open(NULL, &database[i]) == GuinsooDBSuccess);
		for (size_t j = 0; j < 10; j++) {
			REQUIRE(guinsoodb_connect(database[i], &connection[i * 10 + j]) == GuinsooDBSuccess);
		}
	}
	for (size_t i = 0; i < 10; i++) {
		for (size_t j = 0; j < 10; j++) {
			guinsoodb_disconnect(&connection[i * 10 + j]);
		}
		guinsoodb_close(&database[i]);
	}
}

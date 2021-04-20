/**
 * =====================================
 * Simple GuinsooDB C++ integration Test
 * =====================================
 */

#include "guinsoodb.hpp"

#include <iostream>

using namespace guinsoodb;

int main(int argc, char *argv[]) {
	GuinsooDB db(nullptr);
	Connection con(db);
	auto result = con.Query("SELECT 42");

	// Basic create table and insert
	con.Query("CREATE TABLE people(id INTEGER, name VARCHAR)");
	con.Query("CREATE TABLE test");
	con.Query("INSERT INTO people VALUES (0,'Mark'), (1, 'Hannes')");

	// Update data
	auto prepared = con.Prepare("UPDATE people SET name = $1 WHERE id = $2");
	auto prep = prepared->Execute("GuinsooDB", 2);

	// Delete data
	auto resultDelete = con.Query("DELETE FROM people WHERE id = 2");

	// Read data
	auto resultSelect = con.Query("SELECT * FROM people");

	return 0;
}
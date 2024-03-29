#include "catch.hpp"
#include "guinsoodb/common/file_system.hpp"
#include "test_helpers.hpp"

using namespace guinsoodb;
using namespace std;

TEST_CASE("Test functioning of checksum", "[storage]") {
	FileSystem fs;
	unique_ptr<GuinsooDB> database;
	auto storage_database = TestCreatePath("checksum_test");
	auto config = GetTestConfig();

	// make sure the database does not exist
	DeleteDatabase(storage_database);
	{
		// create a database and insert values
		GuinsooDB db(storage_database, config.get());
		Connection con(db);
		REQUIRE_NO_FAIL(con.Query("CREATE TABLE test(a INTEGER);"));
		REQUIRE_NO_FAIL(con.Query("INSERT INTO test VALUES (3);"));
	}
	// we can open the database file now
	REQUIRE_NOTHROW(database = make_unique<GuinsooDB>(storage_database, config.get()));
	database.reset();

	// now write random values into the file
	auto handle = fs.OpenFile(storage_database, FileFlags::FILE_FLAGS_WRITE);
	int8_t value = 0x22;
	fs.Write(*handle, &value, sizeof(int8_t), 100);
	handle->Sync();
	handle.reset();
	// reloading the database no longer works
	REQUIRE_THROWS(database = make_unique<GuinsooDB>(storage_database, config.get()));

	DeleteDatabase(storage_database);
}

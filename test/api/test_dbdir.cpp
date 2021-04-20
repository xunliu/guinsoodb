#include "catch.hpp"
#include "guinsoodb/common/file_system.hpp"
#include "guinsoodb/storage/storage_manager.hpp"
#include "test_helpers.hpp"

using namespace guinsoodb;
using namespace std;

static void test_in_memory_initialization(string dbdir) {
	FileSystem fs;
	unique_ptr<GuinsooDB> db;
	string in_memory_tmp = ".tmp";

	// make sure the temporary folder does not exist
	DeleteDatabase(dbdir);
	fs.RemoveDirectory(in_memory_tmp);

	// cannot create an in-memory database using ":memory:" argument
	REQUIRE_NOTHROW(db = make_unique<GuinsooDB>(dbdir));

	// the temporary folder .tmp should be created in in-memory mode, but was not
	REQUIRE(fs.DirectoryExists(in_memory_tmp));

	// the database dir should not be created in in-memory mode, but was
	REQUIRE(!fs.DirectoryExists(dbdir));

	// clean up
	db.reset();

	// make sure to clean up the database & temporary folder
	DeleteDatabase(dbdir);
	fs.RemoveDirectory(in_memory_tmp);
}

TEST_CASE("Test in-memory database initialization argument \":memory:\"", "[api]") {
	test_in_memory_initialization(":memory:");
}

TEST_CASE("Test in-memory database initialization argument \"\"", "[api]") {
	test_in_memory_initialization("");
}

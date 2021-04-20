#include "guinsoodb.hpp"

using namespace guinsoodb;

int main() {
	GuinsooDB db(nullptr);
	Connection con(db);

	con.Query("CREATE TABLE integers(i INTEGER)");
	con.Query("INSERT INTO integers VALUES (3)");
	auto result = con.Query("SELECT * FROM integers");
	result->Print();
}
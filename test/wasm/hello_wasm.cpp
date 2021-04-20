#include "guinsoodb.hpp"

#include <iostream>

extern "C" {

int main() {
	std::cout << "Hello from WASM" << std::endl;
}

int32_t HelloWasm() {
	guinsoodb::GuinsooDB db(nullptr);
	guinsoodb::Connection con(db);
	auto result = con.Query("CREATE TABLE sometable AS SELECT x FROM generate_series(1,10000) AS a(x)");
	if (!result->success) {
		std::cerr << result->error << std::endl;
		return -1;
	}
	result = con.Query("SELECT sum(x) FROM sometable");
	if (!result->success) {
		std::cerr << result->error << std::endl;
		return -1;
	}
	return result->GetValue(0, 0).value_.bigint;
}
}

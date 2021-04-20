#include "guinsoodb.h"
#include <stdio.h>

int main() {
	guinsoodb_database db = NULL;
	guinsoodb_connection con = NULL;
	guinsoodb_result result;

	if (guinsoodb_open(NULL, &db) == GuinsooDBError) {
		fprintf(stderr, "Failed to open database\n");
		goto cleanup;
	}
	if (guinsoodb_connect(db, &con) == GuinsooDBError) {
		fprintf(stderr, "Failed to open connection\n");
		goto cleanup;
	}
	if (guinsoodb_query(con, "CREATE TABLE integers(i INTEGER, j INTEGER);", NULL) == GuinsooDBError) {
		fprintf(stderr, "Failed to query database\n");
		goto cleanup;
	}
	if (guinsoodb_query(con, "INSERT INTO integers VALUES (3, 4), (5, 6), (7, NULL);", NULL) == GuinsooDBError) {
		fprintf(stderr, "Failed to query database\n");
		goto cleanup;
	}
	if (guinsoodb_query(con, "SELECT * FROM integers", &result) == GuinsooDBError) {
		fprintf(stderr, "Failed to query database\n");
		goto cleanup;
	}
	// print the names of the result
	for (size_t i = 0; i < result.column_count; i++) {
		printf("%s ", result.columns[i].name);
	}
	printf("\n");
	// print the data of the result
	for (size_t row_idx = 0; row_idx < result.row_count; row_idx++) {
		for (size_t col_idx = 0; col_idx < result.column_count; col_idx++) {
			char *val = guinsoodb_value_varchar(&result, col_idx, row_idx);
			printf("%s ", val);
			free(val);
		}
		printf("\n");
	}
	// guinsoodb_print_result(result);
cleanup:
	guinsoodb_destroy_result(&result);
	guinsoodb_disconnect(&con);
	guinsoodb_close(&db);
}

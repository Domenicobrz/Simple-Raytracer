#include "Database.h"
#include <stdlib.h>
#include <string>


Database::Database() { 
	zErrMsg = 0;
	rc;

	rc = sqlite3_open("..\\saved scenes\\test.db", &db);

	if (rc) { fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db)); }
	else { fprintf(stderr, "Opened database successfully\n"); }
};

bool Database::saveResult(int width, int height, int samples, float* buffer, int bufferSize) {

	/* delete pre-existing tables */
	char* sql = "drop table if exists ResultOptions";
	rc = sqlite3_exec(db, sql, nullptr, 0, &zErrMsg);
	checkErr(rc, "table dropped successfully\n");

	sql = "drop table if exists ResultBlob";
	rc = sqlite3_exec(db, sql, nullptr, 0, &zErrMsg);
	checkErr(rc, "table dropped successfully\n");


	/* creating tables */
	sql = "CREATE TABLE ResultOptions("   \
		"WIDTH          INT    NOT NULL," \
		"HEIGHT         INT    NOT NULL," \
		"SAMPLES		INT	   NOT NULL"  \
		");";
	rc = sqlite3_exec(db, sql, nullptr, 0, &zErrMsg);
	checkErr(rc, "table created successfully\n");

	sql = "CREATE TABLE ResultBlob("   \
		"BUFFER			BLOB" \
		");";
	rc = sqlite3_exec(db, sql, nullptr, 0, &zErrMsg);
	checkErr(rc, "table created successfully\n");






	/* actual saving */
	std::string strsql = "INSERT INTO ResultOptions (WIDTH,HEIGHT,SAMPLES) " \
		"VALUES (" + std::to_string(width) + "," + std::to_string(height) + "," + std::to_string(samples) + ");";
	rc = sqlite3_exec(db, strsql.c_str(), nullptr, 0, &zErrMsg);
	checkErr(rc, "Result Options saved successfully\n");


	sqlite3_stmt *stmt = NULL;
	rc = sqlite3_prepare_v2(db,
		"INSERT INTO ResultBlob(BUFFER)"
		" VALUES(?)", -1, &stmt, NULL);

	if (rc != SQLITE_OK) { printf("prepare failed: %s", sqlite3_errmsg(db));  }
	else {
		// SQLITE_STATIC because the statement is finalized
		// before the buffer is freed:
		rc = sqlite3_bind_blob(stmt, 1, buffer, bufferSize, SQLITE_STATIC);
		if (rc != SQLITE_OK) { printf("bind failed: %s", sqlite3_errmsg(db)); }
		else {
			rc = sqlite3_step(stmt);
			if (rc != SQLITE_DONE) printf("execution failed: %s", sqlite3_errmsg(db));
		}
	}
	sqlite3_finalize(stmt);



	sqlite3_close(db);
	return false;
};

bool Database::checkErr(int rc, char* successMessage) {
	if (rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		fprintf(stdout, successMessage);
		return true;
	}

	return false;
};
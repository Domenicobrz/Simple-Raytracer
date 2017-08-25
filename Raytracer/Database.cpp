#include "Database.h"
#include <stdlib.h>
#include <string>
#include <fstream>
#include "Display.h"
#include <stdexcept>

Database::Database() { 
	zErrMsg = 0;
};

bool Database::database_exist(const char *fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

int Database::loadOptionsCallback(void *data, int argc, char **argv, char **azColName) {
	int i;
	RenderOptions* options = (RenderOptions*)data;

	//for (i = 0; i < argc; i++) {
		//printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	//}
	options->width   = atoi(argv[0]);
	options->height  = atoi(argv[1]);
	options->samples = atoi(argv[2]);

	return 0;
}

bool Database::checkDatabaseOpened() {
	if (db != nullptr) return true;

	rc = sqlite3_open(databasePath, &db);

	if (rc) { fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db)); return false; }
	else { fprintf(stderr, "Opened database successfully\n"); return true; }
}

RenderOptions Database::loadRenderOptions() {
	if (!checkDatabaseOpened()) {
		throw std::runtime_error("Can't open Database");
	}

	/* Create SQL statement */
	char *sql = "SELECT * from ResultOptions";

	RenderOptions options;

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, &Database::loadOptionsCallback, (void*)&options, &zErrMsg);
	checkErr(rc, "ResultOptions loaded successfully\n");

	return options;
}

bool Database::loadRenderBlob(float* buffer) {
	if (!checkDatabaseOpened()) return false;

	sqlite3_stmt *statement;
	char* sql = "SELECT * FROM ResultBlob";
	if (sqlite3_prepare_v2(db, sql, strlen(sql), &statement, 0) != SQLITE_OK) {
		printf("Render Load failed \n");
		return false;
	}
	while (true) {
		rc = sqlite3_step(statement);

		if (rc == SQLITE_ROW) {
			// Get the size of the vector
			int size = sqlite3_column_bytes(statement, 0);
			// Get the pointer to data
			unsigned char* p = (unsigned char*)sqlite3_column_blob(statement, 0);
			memcpy((void*)buffer, (void*)p, size);
		}
		else {
			break;
		}
	}

	return true;
}


bool Database::saveRender(int width, int height, int samples, float* buffer, int bufferSize) {
	if (!checkDatabaseOpened()) return false;

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
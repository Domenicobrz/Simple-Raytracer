#pragma once
#include "sqlite3.h"
#include <stdio.h>

/* 

	To load a Render, simply place the database file inside the saved scenes folder, 
	it will be automatically picked up by the program

*/



struct RenderOptions {
	int width;
	int height;
	int samples;
};


class Display;
class Database {
public:
	Database();
	RenderOptions loadRenderOptions();
	bool loadRenderBlob(float*);
	bool saveRender(int, int, int, float*, int);
	bool checkErr(int, char*);

	bool checkDatabaseOpened();

	char* databasePath = "..\\saved scenes\\test.db";

	static bool database_exist(const char *);
	static int loadOptionsCallback(void *data, int argc, char **argv, char **azColName);

private:
	sqlite3* db = nullptr;
	char* zErrMsg = 0;
	int rc;
};
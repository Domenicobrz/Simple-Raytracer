#pragma once
#include "sqlite3.h"
#include <stdio.h>

class Database {
public:
	Database();
	bool saveResult(int, int, int, float*, int);
	bool checkErr(int, char*);


private:
	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
};
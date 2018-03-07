#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "bitset"
#include <utility>
#include "Utils.h"

using namespace std;

void InsertValuesToMap(vector<pair<string, string>>  * fields, vector<string> columns, vector<string> values);
FreeIndexData GetPosForRegister(int initialBlock, int registerLength, string databaseName);
bool InsertToDisk(TableRegister registerT, string databaseName);
bool WriteRowToBlock(vector<pair<string, string>> fields, vector<int> sizes, int blockToWrite, int posToWrite, int idRow, string databaseName);
bool InsertRegister(std::vector<string> list);

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <bitset>
#include <fstream>
#include <unistd.h>
#include "BasicFileSystem.h"

using namespace std;

int GetPosForNewTable(char * buffer);
char * GetTableIndexData(string fileName, int tableIndexBlock);
void printList(std::vector<string> list);
string MixColumnsAndTypes(std::vector<string> columns, std::vector<string> types);
string GetFields(string columns, string types);bool CreateTable(std::vector<string> list);
bool InsertRegister(std::vector<string> list);

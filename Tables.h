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
int GetPosForNewData(char * buffer, int registerLength);
string MixColumnsAndTypes(std::vector<string> columns, std::vector<string> types);
string GetFields(string columns, string types);
bool CreateTable(std::vector<string> list);

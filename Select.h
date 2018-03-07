#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "bitset"
#include <utility>

using namespace std;

bool SelectFromTable(vector<string> list);
void CopyColumns(vector<pair<string, string>> fieldsWithColumn, std::vector<std::pair<std::string, std::string>> *emptyFields);
void PrintSeconds(vector<pair<string, string>> fields);

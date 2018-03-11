#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "bitset"
#include <utility>
#include "Insert.h"



using namespace std;

bool UpdateRegisters(std::vector<string> list);
void UpdateResults(TableRegister table, vector<string> columns, vector<string> values, string condition, string databaseName);

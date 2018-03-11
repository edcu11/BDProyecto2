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

bool DeleteRegisters(std::vector<string> list);
void DeleteResults(TableRegister table, string condition, string databaseName);
bool DropTable(std::vector<string> list);
void DeleteBlocks(int startingBlock, string databaseName);

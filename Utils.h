#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "bitset"
#include "BasicFileSystem.h"
#include <utility>      // std::pair, std::make_pair

class FreeIndexData{

  public:
    FreeIndexData()
    {
      buffer = (char*)calloc(4096,1);
    }

    char * buffer;
    int index;
    int block;
};


bool SContains(std::string word, std::string searched);
std::string Actualsub(std::string word, int from, int to);
std::vector<std::string> SplitWord(std::string sentence, char splitter);
std::vector<std::string> ParseParams(std::string params);
std::string RemoveUntil(std::string word, std::string letter);
std::string GetUntil(std::string word, std::string letter);
std::bitset<8> GetBitSet(char * buffer, int pos);
FreeIndexData GetFreeBlock(std::string fileName);
int GetBitmapBlocksAmount(std::string fileName);
int keyAt(std::vector<std::pair<std::string, std::string>> fields, std::string index);
void PrintPairList(std::vector<std::pair<std::string, std::string>> fields);


class TableRegister{

  public:
    TableRegister()
    {
      tableName = "";
      tableId = -1;
      initialBlock = 0;
    }

    TableRegister(int id, int initialB,  char * name, char * params)
    {
      tableId = id;
      initialBlock = initialB;
      tableName = name;

      std::vector<std::string> pars = SplitWord(std::string(params), ',');

      for (size_t i = 0; i < pars.size(); i++) {
        fields.push_back(std::make_pair(pars[i], ""));
      }

      int removeAt = keyAt(fields, "$$");
      fields.erase(fields.begin() + removeAt);


    }

    std::string tableName;
    int tableId;
    int initialBlock;
    std::vector<std::pair<std::string, std::string>> fields;
};

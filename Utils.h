#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "bitset"
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

using namespace std;

void printList(std::vector<string> list);
char * GetTableIndexData(string fileName, int tableIndexBlock);
int GetTypeSize(string type);
vector<int> GetRegSize(vector<pair<string, string>> fields);
int SumSizes(vector<int> size);
bool SContains(string word, string searched);
string Actualsub(string word, int from, int to);
string TrimWord(string word);
std::vector<string> SplitWord(string sentence, char splitter);
std::bitset<8> GetBitSet(char * buffer, int pos);
FreeIndexData GetFreeBlock(string fileName);
std::vector<std::string> ParseParams(std::string params);
std::string RemoveUntil(std::string word, std::string letter);
std::string GetUntil(std::string word, std::string letter);
int GetBitmapBlocksAmount(string fileName);
void PrintPairList(std::vector<std::pair<std::string, std::string>> fields);
int keyAt(std::vector<std::pair<std::string, std::string>> fields, string key);
string GetColumnsType(string field);
void CopyColumns(vector<pair<string, string>> fieldsWithColumn, std::vector<std::pair<std::string, std::string>> *emptyFields);
void CopyFields(vector<pair<string, string>> fieldsWithColumn, std::vector<std::pair<std::string, std::string>> *emptyFields);
bool WhereClauseIsTrue(vector<pair<string, string>> fields, string condition);
bool ConditionalResult(string condition, string conditionValue, string value);
bool MinorGreaterConverter(string conditionValue, string value, char minor);




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

    void LoadFields(char * rowData, int regAt)
    {
      string columnType = "";
      int typeSize = 0;
      int bytesRead = 4;
      int valueI = 0;
      double valueD = 0;
      char * valueS = (char *)calloc(4000,1);
      //std::cout << "HEY IM REQADING AT: "<< regAt << "  -  "<< bytesRead << '\n';


      for (size_t i = 0; i < fields.size(); i++) {
        columnType = GetColumnsType(fields[i].first);
        typeSize = GetTypeSize(columnType);
        //std::cout << "HEY IM REQADING AT: "<< regAt << "  -  "<< bytesRead << '\n';
        if (SContains(columnType, "int")) {
          memcpy(&valueI, rowData + regAt + bytesRead, sizeof(int));
          bytesRead += sizeof(int);
          fields[i].second = to_string(valueI);
          //std::cout << "Copied int to field: " << fields[i].second << '\n';
          continue;
        }
        if (SContains(columnType, "double")) {
          memcpy(&valueD, rowData + regAt + bytesRead, sizeof(double));
          bytesRead += sizeof(double);
          fields[i].second = to_string(valueD);
          //std::cout << "Copied double to field: " << fields[i].second << '\n';
          continue;
        }
        if(SContains(columnType, "char"))
        {
          memcpy(valueS, rowData + regAt + bytesRead, typeSize); //asi leo strings del archivo
          bytesRead += typeSize;
          fields[i].second = string(valueS);
        //  std::cout << "Copied cjar to field: " << fields[i].second << '\n';
          continue;
        }
      }
    }

};

TableRegister GetTableRegister(char * indexTableData, string tableName);

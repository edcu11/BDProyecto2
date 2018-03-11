#include "Utils.h"
#include <string.h>
#include "BasicFileSystem.h"


#ifndef BLOCK_SIZE
#define BLOCK_SIZE 4096
#endif
#ifndef TABLE_REGISTER_SIZE
#define TABLE_REGISTER_SIZE 150
#endif

#ifndef TABLE_REGISTER_MD_SIZE
#define TABLE_REGISTER_MD_SIZE 36
#endif


using namespace std;

void CopyColumns(vector<pair<string, string>> fieldsWithColumn, std::vector<std::pair<std::string, std::string>> *emptyFields)
{
  for (size_t i = 0; i < fieldsWithColumn.size(); i++) {
    emptyFields->push_back(make_pair(fieldsWithColumn[i].first,""));
  }
}

void CopyFields(vector<pair<string, string>> fieldsWithColumn, std::vector<std::pair<std::string, std::string>> *emptyFields)
{
  for (size_t i = 0; i < fieldsWithColumn.size(); i++) {
    
  }
}


TableRegister GetTableRegister(char * indexTableData, string tableName)
{

  char * regTableName = (char *)calloc(28,1);
  int idTable = 0;
  int initialBlock = 0;
  //std::cout << "table name pursuited: " << tableName << '\n';
  for (size_t i = 0; i < 20; i++) {
    memcpy(&idTable, indexTableData + (i * TABLE_REGISTER_SIZE), sizeof(int));
    memcpy(regTableName, indexTableData + ((i * TABLE_REGISTER_SIZE) + sizeof(int)), 28); //asi leo strings del archivo
    memcpy(&initialBlock, indexTableData + (i * TABLE_REGISTER_SIZE) + 32, sizeof(int));

  //  std::cout << "regTableName:  " << idTable << " - "<< regTableName << "  --  " <<'\n';

    if (SContains(string(regTableName), tableName)) {
      char * params = (char *)calloc(64,1);
      memcpy(params, indexTableData + ((i * TABLE_REGISTER_SIZE) + TABLE_REGISTER_MD_SIZE), 64); //asi leo strings del archivo
    //  std::cout << "Found table params:  " << idTable << " - " << regTableName << " - " << initialBlock << "  --  "<< params <<'\n';

      return TableRegister(idTable, initialBlock, regTableName, params);
    }

  }

  std::cout << "Table doesnt exists!" << '\n';
  return TableRegister();
}


void printList(std::vector<string> list)
{
  for (size_t i = 0; i < list.size(); i++) {
    //std::cout << "item; " << list[i] << '\n';
  }
}

string GetColumnsType(string field)
{
  return SplitWord(field, '!')[1];
}



char * GetTableIndexData(string fileName, int tableIndexBlock)
{
  char *buffer = (char *)calloc(BLOCK_SIZE, 1);
  readFile(tableIndexBlock, buffer, (char *)fileName.c_str());
  return buffer;
}

int GetTypeSize(string type)
{

//  std::cout << "analizing type: "<< type << '\n';
  int size = 0;
  if (type.compare("int") == 0)
    size = sizeof(int);
  if (type.compare("double") == 0)
    size = sizeof(double);
  if (SContains(type, "char"))
  {
    type = RemoveUntil(type, "(");
    type = type.erase(type.size() - 1);
    size =  std::stoi( type );
  }

  return size;


}

vector<int> GetRegSize(vector<pair<string, string>> fields)
{
  string type = "";
  vector<int> fieldsSizes;
  for (size_t i = 0; i < fields.size(); i++) {
    type = SplitWord(fields[i].first, '!')[1];
    fieldsSizes.push_back(GetTypeSize(type));
  }

  return fieldsSizes;
}

int SumSizes(vector<int> size)
{
  int total = 0;
  for (size_t i = 0; i < size.size(); i++)
    total += size[i];
  return total;
}


bool SContains(string word, string searched)
{
  if (word.find(searched) != string::npos)
    return true;
  return false;
}

string Actualsub(string word, int from, int to)
{
    string lastword (word, from, to - from);
    return lastword;
}

string TrimWord(string word)
{
  while(word[0] == ' ')
    word = word.substr(1, word.size());

  while(word[word.size() - 1] == ' ')
    word = word.substr(0, word.size() - 1);
  return word;
}

std::vector<string> SplitWord(string sentence, char splitter)
{
  std::vector<string> mylist;
  int begin = 0;
  sentence = TrimWord(sentence);
  sentence += "@";
  int tam = sentence.size();
  for (size_t i = 0; i < tam; i++) {
  //  std::cout << "this: " << sentence[i] <<  " diff from : " << sentence.size() <<'\n';
    if (sentence[i] == splitter || sentence[i] == '@') {
      string word = Actualsub(sentence, begin, i);
    //  std::cout << "word :" <<  word << "   otra:  " << sentence << "  begin: " << begin << "  end: " << i <<'\n';
    if (word.compare("") != 0) {
    //  std::cout << "before trim: " << word << "!" << '\n';
      word = TrimWord(word);
    //  std::cout << "pushing: " << word << "!" << '\n';
      mylist.push_back(word);
    }
      i++;
      begin = i;
    }
  }



  return mylist;
}

std::bitset<8> GetBitSet(char * buffer, int pos)
{
  std::bitset<8> bits(buffer[pos]);
  //  std::cout <<"in func"<< bits << "\n" ;
  return bits;
}

FreeIndexData GetFreeBlock(string fileName)
{

  FreeIndexData freeIndexData ;
  freeIndexData.buffer = (char *)calloc(BLOCK_SIZE,1);
  int bitmapBlocks = GetBitmapBlocksAmount(fileName);

  for (size_t i = 0; i < bitmapBlocks; i++)
  {
    readFile(i + 2, freeIndexData.buffer, (char *)fileName.c_str());
    for (size_t bitsetCount = 0; bitsetCount < BLOCK_SIZE; bitsetCount++)
    {
      std::bitset<8> bits = GetBitSet(freeIndexData.buffer, bitsetCount);
      for (size_t bitsChecked = 0; bitsChecked < 8; bitsChecked++)
      {
        if(bits[bitsChecked] == 0)
        {
          freeIndexData.buffer[bitsetCount] = freeIndexData.buffer[bitsetCount] | (1 << bitsChecked);
          freeIndexData.block = i;
          freeIndexData.index =  bitsChecked + (bitsetCount * 8);
          return freeIndexData;
        }
      }
    }
  }
}


std::vector<std::string> ParseParams(std::string params)
{
  //std::cout << "parsing: " <<  params <<'\n';
  params = RemoveUntil(params, "=");
  return SplitWord(params, ',');
}

std::string RemoveUntil(std::string word, std::string letter)
{
  int indexOfLetter = word.find(letter) + 1;
  return word.substr(indexOfLetter, word.size() - indexOfLetter);
}

std::string GetUntil(std::string word, std::string letter)
{
  int indexOfLetter = word.find(letter);
  return word.substr(0, indexOfLetter);
}


int GetBitmapBlocksAmount(string fileName)
{
  char *buffer = (char *)calloc(BLOCK_SIZE, 1);
  int bitmapBlocks;
  readFile(1, buffer, (char*)fileName.c_str());
  memcpy(&bitmapBlocks, &buffer[0], 4);
  free(buffer);
  return bitmapBlocks;
}

void PrintPairList(std::vector<std::pair<std::string, std::string>> fields)
{
  for (size_t i = 0; i < fields.size(); i++)
    std::cout <<"\t\t" << fields[i].first  << ": " << fields[i].second;
  std::cout << '\n';
}


int keyAt(std::vector<std::pair<std::string, std::string>> fields, string key)
{
  for (size_t i = 0; i < fields.size(); i++) {
    if (SContains(fields[i].first, key))
      return i;
  }
  return -1;
}

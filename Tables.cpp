#include "Tables.h"
#include "Utils.h"

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

int GetPosForNewTable(char * buffer)
{
  int idTable = 1;
  for (size_t i = 0; i < 19; i++) {
    memcpy(&idTable, buffer + (i * TABLE_REGISTER_SIZE), sizeof(int));
    //std::cout << "idTable: " << idTable <<'\n';
    if (idTable == 0) {
      return (i) * TABLE_REGISTER_SIZE;
    }
  }
  std::cout << "Table index is full!" << '\n';
  return -1;

}


char * GetTableIndexData(string fileName, int tableIndexBlock)
{
  char *buffer = (char *)calloc(BLOCK_SIZE, 1);
  readFile(tableIndexBlock, buffer, (char *)fileName.c_str());
  return buffer;
}

void printList(std::vector<string> list)
{
  for (size_t i = 0; i < list.size(); i++) {
    std::cout << "item; " << list[i] << '\n';
  }
}

string MixColumnsAndTypes(std::vector<string> columns, std::vector<string> types)
{
  string result = "";
  for (size_t i = 0; i < columns.size(); i++) {
    result  += columns[i];
    result += "!";
    result += types[i];
    result += ",";
  }
  result += "$$";

  if (result.size() > TABLE_REGISTER_SIZE - TABLE_REGISTER_MD_SIZE) {
    std::cout << "Too many params" << result <<'\n';
    result = "ERROR";
  }

  return result;
}


string GetFields(string columns, string types)
{
    std::vector<string> columnList = ParseParams(columns);
    std::vector<string> typeList = ParseParams(types);

    printList(columnList);
    printList(typeList);

    return MixColumnsAndTypes(columnList, typeList);
}

bool CreateTable(std::vector<string> list)
{
  printList(list);

  int tableIndexBlock = GetBitmapBlocksAmount(list[2]) + 2;
  std::cout << "tableI: " << tableIndexBlock << '\n';

  char * buffer = GetTableIndexData(list[2], tableIndexBlock);
  int posToWriteNewTable = GetPosForNewTable(buffer);
  int idTable = (posToWriteNewTable / TABLE_REGISTER_SIZE) + 1;
  std::cout << "pos free : " << posToWriteNewTable << '\n';

  FreeIndexData freeIndexData = GetFreeBlock(list[2]);
  writeFile(2 + freeIndexData.block, freeIndexData.buffer, (char*)list[2].c_str()); //writing back bitmapBlock modified
  std::cout << "free block = " << freeIndexData.index << '\n';

  if (posToWriteNewTable == -1)
    return false;

  string fieldsLine = GetFields(list[4], list[5]);

  if (SContains(fieldsLine, "ERROR"))
    return false;

  std::cout << "resu " << fieldsLine << '\n';
  memcpy(&buffer[posToWriteNewTable], &idTable, sizeof(int) ); //ASI SE ESCRIBRE UN NUMERO
  memcpy(buffer + posToWriteNewTable + 4, (char *)list[3].c_str(), list[3].size());
  memcpy(&buffer[posToWriteNewTable + 32], &freeIndexData.index, sizeof(int) ); //ASI SE ESCRIBRE UN NUMERO
  memcpy(buffer + posToWriteNewTable + TABLE_REGISTER_MD_SIZE, (char *)fieldsLine.c_str(), fieldsLine.size());

  writeFile(tableIndexBlock, buffer, (char*)list[2].c_str());

  std::cout << "writing : " << buffer << '\n';
  free(buffer);
  return true;




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

    std::cout << "regTableName:  " << idTable << " - "<< regTableName << "  --  " <<'\n';

    if (SContains(string(regTableName), tableName)) {
      char * params = (char *)calloc(64,1);
      memcpy(params, indexTableData + ((i * TABLE_REGISTER_SIZE) + TABLE_REGISTER_MD_SIZE), 64); //asi leo strings del archivo
      std::cout << "Found table params:  " << idTable << " - " << regTableName << " - " << initialBlock << "  --  "<< params <<'\n';

      return TableRegister(idTable, initialBlock, regTableName, params);
    }

  }

  std::cout << "Table doesnt exists!" << '\n';
  return TableRegister();
}

void InsertValuesToMap(vector<pair<string, string>>  * fields, vector<string> columns, vector<string> values)
{
  string columnName = "";
  string value = "";
  int index = 0;

  std::cout << "columns: " << '\n';
  printList(columns);

  for (size_t i = 0; i < columns.size(); i++) {
    columnName = columns[i];
    value = values[i];
    index = keyAt(* fields, columnName);
    std::cout << "got index: " << index << " Looking for: " << columnName <<"!"<< '\n';

    if(index == -1){
      std::cout << "invalid param" << '\n';
      return;
    }

    std::cout << "inserting at: " << (*fields)[index].first << '\n';
    (*fields)[index].second = value;
  }

}


bool InsertRegister(std::vector<string> list)
{

  printList(list);
  int tableIndexBlock = GetBitmapBlocksAmount(list[0]) + 2;
  std::cout << "tableI: " << tableIndexBlock << '\n';

  char * buffer = GetTableIndexData(list[0], tableIndexBlock);
  TableRegister registerT = GetTableRegister(buffer, list[1]);

  PrintPairList(registerT.fields);
  std::vector<std::string> columns = ParseParams(list[2]);
  std::vector<std::string> values = ParseParams(list[3]);

  printList(columns);
  printList(values);

  InsertValuesToMap(&registerT.fields, columns, values);
  PrintPairList(registerT.fields);











  return true;
}

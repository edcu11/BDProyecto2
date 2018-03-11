#include "Select.h"
#include "Utils.h"
#include "BasicFileSystem.h"


using namespace std;

#ifndef BLOCK_SIZE
#define BLOCK_SIZE 4096
#endif

void PrintSeconds(vector<pair<string, string>> fields)
{
  for (size_t i = 0; i < fields.size(); i++) {
    std::cout << fields[i].second <<"   ";
  }
}

void PrintFirst(vector<pair<string, string>> fields)
{
  for (size_t i = 0; i < fields.size(); i++) {
    std::cout << fields[i].first <<"   ";
  }
}

void PrintOnlyColumns(vector<pair<string, string>> fields, string columns)
{
  if (SContains(columns, "*")) {
    PrintPairList(fields);
  }
  std::vector<std::string> columnasAmostrar = ParseParams(columns);
  int index = 0;

  for (size_t i = 0; i < columnasAmostrar.size(); i++) {
    index = keyAt(fields, columnasAmostrar[i]);
    std::cout <<"\t\t" << fields[index].first  << ": " << fields[index].second;
  }
  std::cout << '\n';

}




void PrintSelectResults(TableRegister table, string columns, string databaseName)
{
  TableRegister row = TableRegister(); //FOR THE FIELDS USE ONLY
  int lastByte = -1;
  int readBlock = table.initialBlock;
  int idRow = 0;
  char * buffer = (char *)calloc(BLOCK_SIZE, 1);
  vector<int> regSizes = GetRegSize(table.fields);
  int registerLength = SumSizes(regSizes) + 4;
  CopyColumns(table.fields, &row.fields);


  do {
    readFile(readBlock, buffer, (char *)databaseName.c_str());

    for (size_t i = 0; i < BLOCK_SIZE / registerLength; i++) {
      memcpy(&idRow, buffer + (i * registerLength), sizeof(int));
      if(idRow == 0)
      {
        std::cout << "No more Regs!" << '\n';
        return;
      }
      row.LoadFields(buffer, (i * registerLength));

      PrintOnlyColumns(row.fields, columns);
    }
    memcpy(&lastByte, buffer + (4092), sizeof(int));
    readBlock = lastByte;
  }  while (lastByte != 0);

}



bool SelectFromTable(vector<string> list)
{
  printList(list);
  string databaseName = list[0];
  string tableName = list[1];
  string columns = list[2];

  int tableIndexBlock = GetBitmapBlocksAmount(databaseName) + 2;

  char * buffer = GetTableIndexData(databaseName, tableIndexBlock);
  TableRegister registerT = GetTableRegister(buffer, tableName);
  PrintSelectResults(registerT, columns, databaseName);

  return true;
}

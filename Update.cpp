#include "Update.h"
#include "Utils.h"
#include "BasicFileSystem.h"

#ifndef BLOCK_SIZE
#define BLOCK_SIZE 4096
#endif

using namespace std;

bool UpdateRegisters(std::vector<string> list)
{
  printList(list);
  string databaseName = list[0];
  string tableName = list[1];
  vector<string> columns = ParseParams(list[2]);
  vector<string> values = ParseParams(list[3]);
  string condition = "";

  if (list.size() > 4) {
    condition = list[4];
  }

  int tableIndexBlock = GetBitmapBlocksAmount(databaseName) + 2;
  char * buffer = GetTableIndexData(databaseName, tableIndexBlock);
  TableRegister registerT = GetTableRegister(buffer, tableName);
  UpdateResults(registerT, columns, values, condition, databaseName);

  return true;
}


void UpdateResults(TableRegister table, vector<string> columns, vector<string> values, string condition, string databaseName)
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
        continue;
      }
      row.LoadFields(buffer, (i * registerLength));

      if(!WhereClauseIsTrue(row.fields, condition)){
        continue;
      }

      InsertValuesToMap(&row.fields, columns, values);
      WriteRowToBlock(row.fields, regSizes, readBlock, (i * registerLength), idRow, databaseName);

    }

    memcpy(&lastByte, buffer + (4092), sizeof(int));
    readBlock = lastByte;


  }  while (lastByte != 0);

  std::cout << "Values Updated Succesfully!" << '\n';
}

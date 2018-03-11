#include "Delete.h"
#include "Utils.h"
#include "BasicFileSystem.h"

#ifndef BLOCK_SIZE
#define BLOCK_SIZE 4096
#endif

#ifndef TABLE_REGISTER_SIZE
#define TABLE_REGISTER_SIZE 150
#endif


using namespace std;

bool DeleteRegisters(std::vector<string> list)
{
  printList(list);
  string databaseName = list[0];
  string tableName = list[1];
  string condition = "";
  //std::cout << "table:  " << tableName <<"!"<< '\n';

  if (list.size() > 2) {
    if (list[1].compare(list[2]) != 0)
      condition = list[2];
  }
//  std::cout << "condition "<< condition << '\n';
  int tableIndexBlock = GetBitmapBlocksAmount(databaseName) + 2;
  char * buffer = GetTableIndexData(databaseName, tableIndexBlock);
  TableRegister registerT = GetTableRegister(buffer, tableName);
  DeleteResults(registerT, condition, databaseName);

  return true;
}



void DeleteResults(TableRegister table, string condition, string databaseName)
{
  TableRegister row = TableRegister(); //FOR THE FIELDS USE ONLY
  int lastByte = -1;
  int readBlock = table.initialBlock;
  int idRow = 0;
  char * buffer = (char *)calloc(BLOCK_SIZE, 1);
  vector<int> regSizes = GetRegSize(table.fields);
  int registerLength = SumSizes(regSizes) + 4;
  CopyColumns(table.fields, &row.fields);
  char * emptyBuffer = (char *)calloc(registerLength, 1);


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
      //  std::cout << "continuing;" << '\n';
        continue;
      }

      memcpy(buffer + (i * registerLength), emptyBuffer, registerLength);
      writeFile(readBlock, buffer, (char *)databaseName.c_str());
    }

    memcpy(&lastByte, buffer + (4092), sizeof(int));
    readBlock = lastByte;


  }  while (lastByte != 0);
  std::cout << "Rows Deleted Succesfully!" << '\n';

}

bool DropTable(std::vector<string> list)
{
  printList(list);
  string databaseName = list[0];
  string tableName = list[1];

  int tableIndexBlock = GetBitmapBlocksAmount(databaseName) + 2;
  char * buffer = GetTableIndexData(databaseName, tableIndexBlock);
  TableRegister registerT = GetTableRegister(buffer, tableName);

  char * emptyBuffer = (char *)calloc(TABLE_REGISTER_SIZE, 1);
  int tableIndex = ((registerT.tableId - 1) * TABLE_REGISTER_SIZE);
  memcpy(buffer + tableIndex, emptyBuffer, TABLE_REGISTER_SIZE);
  writeFile(tableIndexBlock, buffer, (char*)databaseName.c_str());

  DeleteBlocks(registerT.initialBlock, databaseName);
  return true;


}


void DeleteBlocks(int startingBlock, string databaseName)
{
  int readBlock = startingBlock;
  int lastByte = 0;
  char * emptyBuffer = (char *)calloc(BLOCK_SIZE, 1);
  char * buffer = (char *)calloc(BLOCK_SIZE, 1);

  do {
    readFile(readBlock, buffer, (char *)databaseName.c_str());
    memcpy(&lastByte, buffer + (4092), sizeof(int));

    writeFile(readBlock, emptyBuffer, (char*)databaseName.c_str());
    readBlock = lastByte;
  }  while (lastByte != 0);

  std::cout << "Table deleted succesfully!" << '\n';

}

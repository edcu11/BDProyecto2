#include "Insert.h"
#include "Tables.h"

#ifndef BLOCK_SIZE
#define BLOCK_SIZE 4096
#endif
#ifndef TABLE_REGISTER_SIZE
#define TABLE_REGISTER_SIZE 150
#endif

#ifndef TABLE_REGISTER_MD_SIZE
#define TABLE_REGISTER_MD_SIZE 36
#endif



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



FreeIndexData GetPosForRegister(int initialBlock, int registerLength, string databaseName)
{

  char * buffer = (char *)calloc(BLOCK_SIZE, 1);
  readFile(initialBlock, buffer, (char *)databaseName.c_str());
  int lastByte = -1;
  int readedBlock = initialBlock;
  FreeIndexData freeIndex;

  while(lastByte != 0)
  {
    memcpy(&lastByte, buffer + (4092), sizeof(int));
    std::cout << "last byte: "<< lastByte << '\n';
    if (lastByte == 0) {
      freeIndex.block = readedBlock;
      freeIndex.index = GetPosForNewData(buffer, registerLength + 4);

      if (freeIndex.index == -1) {
        std::cout << "there was no more space, allocating..." << '\n';
        FreeIndexData bitmapData = GetFreeBlock(databaseName);
        writeFile(2 + bitmapData.block, bitmapData.buffer, (char*)databaseName.c_str()); //writing back bitmapBlock modified

        memcpy(&buffer[BLOCK_SIZE - 4], &bitmapData.index, sizeof(int)); //ORGANIZACION EXTENDIDA
        writeFile(freeIndex.block, buffer, (char*)databaseName.c_str());
        freeIndex.block = bitmapData.index;
        freeIndex.index = 0;
      }
      std::cout << "found pos for new reg: " << freeIndex.block << " --- " << freeIndex.index << '\n';
      free(buffer);
      return freeIndex;
    }
    readFile(lastByte, buffer, (char *)databaseName.c_str());
    readedBlock = lastByte;
    std::cout << "read new buffer!" << '\n';
  }

  return freeIndex;

}

bool InsertToDisk(TableRegister registerT, string databaseName)
{

  vector<int> regSizes = GetRegSize(registerT.fields);
  int totalSize = SumSizes(regSizes);

  FreeIndexData freeIndex = GetPosForRegister(registerT.initialBlock, totalSize, databaseName);

  int idRow = (freeIndex.index / totalSize) + 1;
  std::cout << "posToWrite: "<< freeIndex.index << " -- " << idRow << " -- " << freeIndex.block << '\n';

  WriteRowToBlock(registerT.fields, regSizes, freeIndex.block, freeIndex.index, idRow, databaseName);

}

bool WriteRowToBlock(vector<pair<string, string>> fields, vector<int> sizes, int blockToWrite, int posToWrite, int idRow, string databaseName)
{
  char * buffer = (char *)calloc(BLOCK_SIZE, 1);
  readFile(blockToWrite, buffer, (char *)databaseName.c_str());

  int valueI = 0;
  double valueD = 0.0;
  int bytesWritten = 4;

  memcpy(&buffer[posToWrite], &idRow, sizeof(int)); //ASI SE ESCRIBRE UN NUMERO

  for (size_t i = 0; i < fields.size(); i++) {
    std::cout << "\t\t iteracion" << '\n';
    if (SContains(fields[i].first, "int")) {
      valueI = std::stoi(fields[i].second);
      memcpy(&buffer[posToWrite + bytesWritten], &valueI, sizeof(int)); //ASI SE ESCRIBRE UN NUMERO
      bytesWritten += sizeof(int);
      std::cout << "writing int: "<< valueI << " bytesWritten: " << " at: " << (posToWrite + bytesWritten) << "     "<< bytesWritten << '\n';
      continue;
    }
    if (SContains(fields[i].first, "double")) {
      valueD = atof((fields[i].second).c_str());
      memcpy(&buffer[posToWrite + bytesWritten], &valueD, sizeof(double)); //ASI SE ESCRIBRE UN NUMERO
      bytesWritten += sizeof(double);
      std::cout << "writing double: "<< valueD << " bytesWritten: "<< " at: " << (posToWrite + bytesWritten) << "     "<< bytesWritten << '\n';
      continue;
    }
    if(SContains(fields[i].first, "char"))
    {
      char  * valor = (char *)fields[i].second.c_str();
      memcpy(buffer + bytesWritten, valor, fields[i].second.size());
      bytesWritten += sizes[i];
      std::cout << "writing char: "<< fields[i].second << "size: " << fields[i].second.size() << " bytesWritten: "<< " at: " << (posToWrite + bytesWritten) << "     "<< bytesWritten << '\n';
      continue;
    }
    std::cout << "nothing bruh" << '\n';
  }

  writeFile(blockToWrite, buffer, (char *)databaseName.c_str());
}

bool InsertRegister(std::vector<string> list)
{

  printList(list);
  string databaseName = list[0];
  string tableName = list[1];
  string columnsParams = list[2];
  string valuesParams = list[3];


  int tableIndexBlock = GetBitmapBlocksAmount(databaseName) + 2;
  std::cout << "tableI: " << tableIndexBlock << '\n';

  char * buffer = GetTableIndexData(databaseName, tableIndexBlock);
  TableRegister registerT = GetTableRegister(buffer, tableName);

  PrintPairList(registerT.fields);
  std::vector<std::string> columns = ParseParams(columnsParams);
  std::vector<std::string> values = ParseParams(valuesParams);

  printList(columns);
  printList(values);

  InsertValuesToMap(&registerT.fields, columns, values);
  PrintPairList(registerT.fields);

  InsertToDisk(registerT, databaseName);

  return true;
}

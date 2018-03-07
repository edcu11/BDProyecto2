#include "Tables.h"
#include "Utils.h"
#include <math.h>

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
//RECORDA QUE CAMBIASTE ESTA MIRDA PRPBA CREAR TABLA DE NUEVO FUCK
int GetPosForNewData(char * buffer, int registerLength)
{
  int idTable = 1;
  int cantidadAceptable = floor(BLOCK_SIZE / registerLength);
  std::cout << "cantidad cantidadAceptable" << cantidadAceptable << '\n';
  for (size_t i = 0; i < cantidadAceptable; i++) {
    memcpy(&idTable, buffer + (i * registerLength), sizeof(int));
    //std::cout << "idTable: " << idTable <<'\n';
    if (idTable == 0) {
      return (i) * registerLength;
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
  int posToWriteNewTable = GetPosForNewData(buffer, TABLE_REGISTER_SIZE);
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

int GetTypeSize(string type)
{

  std::cout << "analizing type: "<< type << '\n';
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
      return freeIndex;
    }
    readFile(lastByte, buffer, (char *)databaseName.c_str());
    readedBlock = lastByte;
    std::cout << "read new buffer!" << '\n';
  }

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
    if (sizes[i] == sizeof(int)) {
      valueI = std::stoi(fields[i].second);
      memcpy(&buffer[posToWrite + bytesWritten], &valueI, sizeof(int)); //ASI SE ESCRIBRE UN NUMERO
      bytesWritten += sizeof(int);
      std::cout << "writing: "<< valueI << " bytesWritten: " << " at: " << (posToWrite + bytesWritten) << "     "<< bytesWritten << '\n';
      continue;
    }
    if (sizes[i] == sizeof(double)) {
      valueD = atof((fields[i].second).c_str());
      memcpy(&buffer[posToWrite + bytesWritten], &valueD, sizeof(double)); //ASI SE ESCRIBRE UN NUMERO
      bytesWritten += sizeof(double);
      std::cout << "writing: "<< valueD << " bytesWritten: "<< " at: " << (posToWrite + bytesWritten) << "     "<< bytesWritten << '\n';
      continue;
    }
    else
    {
      char  * valor = (char *)fields[i].second.c_str();
      memcpy(buffer + bytesWritten, valor, fields[i].second.size());
      bytesWritten += sizes[i];
      std::cout << "writing: "<< fields[i].second << "size: " << fields[i].second.size() << " bytesWritten: "<< " at: " << (posToWrite + bytesWritten) << "     "<< bytesWritten << '\n';
    }
  }

  writeFile(blockToWrite, buffer, (char *)databaseName.c_str());
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

  InsertToDisk(registerT, list[0]);

  return true;
}

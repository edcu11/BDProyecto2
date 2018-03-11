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
  //std::cout << "cantidad cantidadAceptable" << cantidadAceptable << '\n';
  for (size_t i = 0; i < cantidadAceptable; i++) {
    memcpy(&idTable, buffer + (i * registerLength), sizeof(int));
    //std::cout << "idTable: " << idTable <<'\n';
    if (idTable == 0) {
      return (i) * registerLength;
    }
  }
  //std::cout << "Table index is full!" << '\n';
  return -1;

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
  //std::cout << "tableI: " << tableIndexBlock << '\n';

  char * buffer = GetTableIndexData(list[2], tableIndexBlock);
  int posToWriteNewTable = GetPosForNewData(buffer, TABLE_REGISTER_SIZE);
  int idTable = (posToWriteNewTable / TABLE_REGISTER_SIZE) + 1;
  //std::cout << "pos free : " << posToWriteNewTable << '\n';

  FreeIndexData freeIndexData = GetFreeBlock(list[2]);
  writeFile(2 + freeIndexData.block, freeIndexData.buffer, (char*)list[2].c_str()); //writing back bitmapBlock modified
  //std::cout << "free block = " << freeIndexData.index << '\n';

  if (posToWriteNewTable == -1)
    return false;

  string fieldsLine = GetFields(list[4], list[5]);

  if (SContains(fieldsLine, "ERROR"))
    return false;

  //std::cout << "resu " << fieldsLine << '\n';
  memcpy(&buffer[posToWriteNewTable], &idTable, sizeof(int) ); //ASI SE ESCRIBRE UN NUMERO
  memcpy(buffer + posToWriteNewTable + 4, (char *)list[3].c_str(), list[3].size());
  memcpy(&buffer[posToWriteNewTable + 32], &freeIndexData.index, sizeof(int) ); //ASI SE ESCRIBRE UN NUMERO
  memcpy(buffer + posToWriteNewTable + TABLE_REGISTER_MD_SIZE, (char *)fieldsLine.c_str(), fieldsLine.size());

  writeFile(tableIndexBlock, buffer, (char*)list[2].c_str());

  //std::cout << "writing : " << buffer << '\n';
  free(buffer);
  return true;

}

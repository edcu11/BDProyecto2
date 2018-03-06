#include "BasicFileSystem.h"


#ifndef BLOCK_SIZE
#define BLOCK_SIZE 4096
#endif

void imprimir()
{
  printf("asdasd\n");
}

int writeFile(int address, char * buffer, char * fileName)
{

  std::fstream fs;
  fs.open(fileName, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
  fs.seekp(address * BLOCK_SIZE, std::ios::beg);
  fs.write(buffer,4096);
  fs.close();

  return 0;
}

int readFile(int address, char * buffer, char * fileName)
{

  std::fstream fs;
  fs.open(fileName, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
  fs.seekp(address * BLOCK_SIZE, std::ios::beg);
  fs.read(buffer, 4096);
  fs.close();

}

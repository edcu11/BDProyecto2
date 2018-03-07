#include "Select.h"

using namespace std;

bool SelectFromTable(std::vector<string> list)
{
  int tableIndexBlock = GetBitmapBlocksAmount(list[2]) + 2;
  std::cout << "tableI: " << tableIndexBlock << '\n';

  char * buffer = GetTableIndexData(list[2], tableIndexBlock);



}

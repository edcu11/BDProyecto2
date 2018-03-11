#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <bitset>
#include <fstream>
#include <unistd.h>
#include "CreateDisk.h"
#include "Tables.h"
#include "Insert.h"
#include "Select.h"
#include "Update.h"

using namespace std;

string getInput()
{
    string valor = " ";
    getline(cin, valor);
    return valor;
}

int main(int argc, char const *argv[]) {

  string menu = "";
  int x = 3;
  string connectedDatabase = "kamil.dbo";


  while (menu.compare("exit") != 0) {

    if(SContains(menu, "create database") )
    {
      //menu = menu.substr(menu.find("database") + 8, menu.size() - 1);
      std::vector<string> lista = SplitWord(menu, ' ');
      CrearDisco(lista);
      std::cout << "Database created!" << '\n';
    }

    if(SContains(menu, "create table") )
    {
      std::vector<string> lista = SplitWord(menu, ' ');

      lista.insert(lista.begin() + 2, connectedDatabase);
      CreateTable(lista);
    }

    if(SContains(menu, "insert") )
    {
      menu = RemoveUntil(menu, " ");
      std::string tableName = GetUntil(menu, " ");
      menu = RemoveUntil(menu, " ");
      std::cout << "menus: " << menu << '\n';
      std::vector<string> lista = SplitWord(menu, '-');

      lista.insert(lista.begin(), connectedDatabase);
      lista.insert(lista.begin() + 1, tableName);
      InsertRegister(lista);
    }

    if(SContains(menu, "select") )
    {
      menu = RemoveUntil(menu, " ");
      std::string tableName = GetUntil(menu, " ");
      menu = RemoveUntil(menu, " ");
      std::cout << "menus: " << menu << '\n';
      std::vector<string> lista = SplitWord(menu, '-');

      lista.insert(lista.begin(), connectedDatabase);
      lista.insert(lista.begin() + 1, tableName);
      SelectFromTable(lista);
    }

    if(SContains(menu, "update") )
    {
      menu = RemoveUntil(menu, " ");
      std::string tableName = GetUntil(menu, " ");
      menu = RemoveUntil(menu, " ");
      std::cout << "menus: " << menu << '\n';
      std::vector<string> lista = SplitWord(menu, '-');

      lista.insert(lista.begin(), connectedDatabase);
      lista.insert(lista.begin() + 1, tableName);
      UpdateRegisters(lista);
    }




    printf("DBCli# ");
    menu = getInput();
  }
  return 0;
}

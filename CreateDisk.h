#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <math.h>
#include <iostream>
#include <bitset>
#include <fstream>
#include <unistd.h>
#include "BasicFileSystem.h"


//Funcion para cubrir los bloquees ocupados ya que el tamDelDisco es dinamico
int GetSizeMultiplier(char ByteSize);
//Funcion para cubrir los bloquees ocupados ya que el tamDelDisco es dinamico
char * InitBitMap(int cantidadDeBloquees, char * buffer);
long int GetDiskSize(std::string tamano);
bool CrearDisco(std::vector<std::string> argv);
bool CheckDiskExistence(std::string databaseName);

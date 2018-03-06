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
char * InitBitMap(int cantidadDeBloquees, char * buffer);
int GetSizeMultiplier(char ByteSize);
long int GetDiskSize(const char * tamano);
bool CrearDisco(std::vector<std::string> argv);

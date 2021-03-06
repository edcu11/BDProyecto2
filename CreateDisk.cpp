#include "BasicFileSystem.h"
#include "CreateDisk.h"
#include "Utils.h"



int GetSizeMultiplier(char ByteSize)
{
    switch (ByteSize) {
      case 'K':
        return 1024;
      case 'M':
        return 1024 * 1024;
      case 'G':
        return 1024 * 1024 * 1024;
    }
}

//Funcion para cubrir los bloquees ocupados ya que el tamDelDisco es dinamico
char * InitBitMap(int cantidadDeBloquees, char * buffer)
{
  int uno = 1;
  int byte = 0;
  for (size_t i = 0; i < (cantidadDeBloquees + 2); i++) {
    byte = floor(i/8.0f);
    buffer[byte] = buffer[byte] | (uno << (i%8) );
  }


}

long int GetDiskSize(std::string tamano)
{
  std::string numberOnly = tamano;
  int tamanoDeString = numberOnly.size();// strlen(tamano);
  int Multiplier = GetSizeMultiplier(tamano[tamanoDeString - 2]);
  numberOnly.resize(tamanoDeString - 2);
  return (long int)std::stoi(numberOnly.c_str()) * Multiplier;

}

bool CrearDisco(std::vector<std::string> argv)
{

  long int diskSize = GetDiskSize(argv[3]);
  float blockNumber = diskSize / 4096.0f;
  float BitmapBits = blockNumber / 8.0f;
  int BitmapBlocks = ceil(BitmapBits / 4096.0f);

  std::ofstream ofs (argv[2], std::ofstream::out);

  char* fbuffer = (char *)calloc(4096,1);
  ofs.seekp(diskSize, std::ios::beg);
  ofs.write("",1);
  ofs.close();

  memcpy(&fbuffer[sizeof(int) * 0], &BitmapBlocks, sizeof(int) );
  memcpy(&fbuffer[sizeof(int) * 1], &diskSize, sizeof(diskSize) );
  memcpy(fbuffer + 64, (char *)argv[2].c_str(), argv[2].size()); //ASI SE COPIA UN STRING
  writeFile(1, fbuffer, (char *)argv[2].c_str());

  char * bitMapBuffer = (char *) calloc(4096  * (BitmapBlocks + 2), 1 );
  InitBitMap(BitmapBlocks + 2, bitMapBuffer);

  //memset(bitMapBuffer, 1, 4096 * BitmapBlocks);


  for (size_t blocksCopied = 0; blocksCopied < BitmapBlocks; blocksCopied++) {
    memmove(fbuffer, bitMapBuffer + (blocksCopied *  4096), 4096);
    writeFile(2 + blocksCopied, fbuffer, (char *)argv[2].c_str());
   }

     std::cout << "created disk with size: " << diskSize << '\n';

   return true;
 }



 bool CheckDiskExistence(string databaseName)
 {
   char* fbuffer = (char *)calloc(4096,1);
   readFile(1, fbuffer, (char *)databaseName.c_str());
   char * valueS = (char *)calloc(databaseName.size(),1);
   memcpy(valueS, fbuffer + 64, databaseName.size()); //asi leo strings del archivo
   string name = string(valueS);

   if (databaseName.compare(name) == 0) {
     std::cout << "Database Succesfully connected!" << '\n';
     return true;
   }
   std::cout << "Unable to connect to Database!" << '\n';
   return false;






 }

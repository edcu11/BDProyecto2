#include "Utils.h"
#include <string.h>


#ifndef BLOCK_SIZE
#define BLOCK_SIZE 4096
#endif

using namespace std;




bool SContains(string word, string searched)
{
  if (word.find(searched) != string::npos)
    return true;
  return false;
}

string Actualsub(string word, int from, int to)
{
    string lastword (word, from, to - from);
    return lastword;
}

string TrimWord(string word)
{
  while(word[0] == ' ')
    word = word.substr(1, word.size());

  while(word[word.size() - 1] == ' ')
    word = word.substr(0, word.size() - 1);
  return word;
}

std::vector<string> SplitWord(string sentence, char splitter)
{
  std::vector<string> mylist;
  int begin = 0;
  sentence = TrimWord(sentence);
  sentence += "@";
  int tam = sentence.size();
  for (size_t i = 0; i < tam; i++) {
  //  std::cout << "this: " << sentence[i] <<  " diff from : " << sentence.size() <<'\n';
    if (sentence[i] == splitter || sentence[i] == '@') {
      string word = Actualsub(sentence, begin, i);
    //  std::cout << "word :" <<  word << "   otra:  " << sentence << "  begin: " << begin << "  end: " << i <<'\n';
    if (word.compare("") != 0) {
    //  std::cout << "before trim: " << word << "!" << '\n';
      word = TrimWord(word);
    //  std::cout << "pushing: " << word << "!" << '\n';
      mylist.push_back(word);
    }
      i++;
      begin = i;
    }
  }



  return mylist;
}

std::bitset<8> GetBitSet(char * buffer, int pos)
{
  std::bitset<8> bits(buffer[pos]);
  //  std::cout <<"in func"<< bits << "\n" ;
  return bits;
}

FreeIndexData GetFreeBlock(string fileName)
{

  FreeIndexData freeIndexData ;
  freeIndexData.buffer = (char *)calloc(BLOCK_SIZE,1);
  int bitmapBlocks = GetBitmapBlocksAmount(fileName);

  for (size_t i = 0; i < bitmapBlocks; i++)
  {
    readFile(i + 2, freeIndexData.buffer, (char *)fileName.c_str());
    for (size_t bitsetCount = 0; bitsetCount < BLOCK_SIZE; bitsetCount++)
    {
      std::bitset<8> bits = GetBitSet(freeIndexData.buffer, bitsetCount);
      for (size_t bitsChecked = 0; bitsChecked < 8; bitsChecked++)
      {
        if(bits[bitsChecked] == 0)
        {
          freeIndexData.buffer[bitsetCount] = freeIndexData.buffer[bitsetCount] | (1 << bitsChecked);
          freeIndexData.block = i;
          freeIndexData.index =  bitsChecked + (bitsetCount * 8);
          return freeIndexData;
        }
      }
    }
  }
}


std::vector<std::string> ParseParams(std::string params)
{
  //std::cout << "parsing: " <<  params <<'\n';
  params = RemoveUntil(params, "=");
  return SplitWord(params, ',');
}

std::string RemoveUntil(std::string word, std::string letter)
{
  int indexOfLetter = word.find(letter) + 1;
  return word.substr(indexOfLetter, word.size() - indexOfLetter);
}

std::string GetUntil(std::string word, std::string letter)
{
  int indexOfLetter = word.find(letter);
  return word.substr(0, indexOfLetter);
}


int GetBitmapBlocksAmount(string fileName)
{
  char *buffer = (char *)calloc(BLOCK_SIZE, 1);
  int bitmapBlocks;
  readFile(1, buffer, (char*)fileName.c_str());
  memcpy(&bitmapBlocks, &buffer[0], 4);
  free(buffer);
  return bitmapBlocks;
}

void PrintPairList(std::vector<std::pair<std::string, std::string>> fields)
{
  for (size_t i = 0; i < fields.size(); i++)
    std::cout << "fields:   " << fields[i].first  << " -- " << fields[i].second << '\n';

}


int keyAt(std::vector<std::pair<std::string, std::string>> fields, string key)
{
  for (size_t i = 0; i < fields.size(); i++) {
    if (SContains(fields[i].first, key))
      return i;
  }
  return -1;
}

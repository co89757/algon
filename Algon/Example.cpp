#include "dstruct/SkipList.h"
#include "dstruct/DisjointSet.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include "utility/Exception.h"
#include "utility/FileIO.h"
using namespace colinli::algon;
using namespace std;
using namespace colinli;

void test_fileio() {
  file::ReadableFile file("rd.txt");
  string line;
  while (file.ReadLine(line)) {
    cout << line << "\n";
  }

}

void test_except() {
  
    Throwf<InvalidDataException>("a sample error is thrown, the x=%d", 10);
   
}

int main(int argc, char* argv[])
{
  try{
    test_fileio();
  } catch (std::exception& e) {
    cout << e.what() << "\n";
  }
  system("pause");
}

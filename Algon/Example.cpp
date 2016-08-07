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
  auto f = fopen("ii.r", "r");
  CHECK_THROW(f != NULL, FileIOException);
}

void ThrowE(){
  Throwf<InvalidDataException>("a sample error is thrown, the x=%d", 10);

}

void test_except() {
  
  ThrowE();
   
}

int main(int argc, char* argv[])
{
  try{
    test_fileio();
  } catch (exception& e) {
    cout << e.what() << "\n";
  }
  system("pause");
}

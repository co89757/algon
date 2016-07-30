#include "dstruct/SkipList.h"
#include <iostream>
#include <string>
using namespace colinli::algon;
using namespace std;

int main(int argc, char* argv[])
{
  SkipList<int, std::string, 16> list(1000);
  list.Put(1, "one");
  bool haskey = list.HasKey(1);
  string v;
  if (list.TryGet(1,&v))
  {
    cout << "the val = "<< v << "\n";
  }

}

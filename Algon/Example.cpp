#include "dstruct/SkipList.h"
#include "dstruct/DisjointSet.h"
#include <iostream>
#include <string>
using namespace colinli::algon;
using namespace std;

int main(int argc, char* argv[])
{
  SkipList<int, std::string, 16> list;
  list.Put(1, "one");
  bool haskey = list.HasKey(1);
  string v;
  if (list.TryGet(1,&v))
  {
    cout << "the val = "<< v << "\n";
  }
  DisjointSets<string> set;
  set.Add("1");
  set.Add("2");
  set.Add("3");
  set.Union("2", "3");
  auto r = set.Connected("2", "3");
  cout << "done\n";
}

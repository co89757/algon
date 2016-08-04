#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace colinli::algon;
using namespace std;
namespace tests {
TEST_CLASS(TestSkiplist)
{
public:
  SkipList<int, std::string, 16> list;
  TEST_METHOD_INITIALIZE(InitializeSkiplist) {
    list.Put(1, "one");
    list.Put(2, "two");
    list.Put(3, "three");
  }

  TEST_METHOD(TestPutKey) {
    string val;
    bool haskey = list.TryGet(1, &val);
    Assert::IsTrue(haskey);
    Assert::AreEqual("one", val.c_str(), L"value mismatch");
    haskey = list.TryGet(4, &val);
    Assert::IsFalse(haskey);
  }

  TEST_METHOD(TestRemoveKey) {
    bool ret = list.TryRemove(2);
    Assert::IsTrue(ret, L"Key is not present");
    Assert::IsFalse(list.HasKey(2));
    ret = list.TryRemove(5);
    Assert::IsFalse(ret);
  }
};
}

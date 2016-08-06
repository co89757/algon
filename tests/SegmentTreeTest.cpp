#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace colinli::algon;
using namespace std;
namespace tests
{
	TEST_CLASS(SegmentTreeTest)
	{
	public:
		
		TEST_METHOD(TestQueryTree)
		{
      auto min_reducer = [](int x, int y) throw() { return x < y ? x : y; };
      auto identity = [](int x) {return x; };
      int initv = INT_MAX;
      vector<int> x{ 1, 2, 3, 4, 5, 6, 7 };
      SegmentTree<int, int> tree(x, min_reducer, identity, initv);
      Assert::AreEqual(3, tree.Query(2, 5));
      tree.Update(3, -1);
      Assert::AreEqual(-1, tree.Query(1, 6));
		}

	};
}
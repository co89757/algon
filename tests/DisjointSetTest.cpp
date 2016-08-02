#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace colinli::algon;
namespace tests
{
	TEST_CLASS(DisjointSetTest)
	{
	public:
    DisjointSets<int> iset;
    TEST_METHOD_INITIALIZE(InitializeSet)
    {
      for (int i = 0; i < 10; i++)
      {
        iset.Add(i);
      }
    }

		TEST_METHOD(TestContains)
		{
      Assert::IsTrue(iset.Contains(2));
		}
    TEST_METHOD(TestUnion)
    {
      iset.Union(1, 2);
      iset.Union(2, 5);
      iset.Union(3, 5);
      Assert::IsTrue(iset.Connected(1, 2));
      Assert::IsTrue(iset.Connected(1, 5));
      Assert::IsTrue(iset.Connected(1, 3));
      Assert::IsTrue(iset.Connected(2, 3));
    }

    TEST_METHOD(TestMultiUnion)
    {
      iset.Union(1, 2, 3, 4);
      iset.Union(2, 7);
      Assert::IsTrue(iset.Connected(1, 7));
      Assert::IsTrue(iset.Connected(4, 7));
      Assert::IsTrue(iset.Connected(4, 3));
    }
     

	};
}
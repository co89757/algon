#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{
	TEST_CLASS(NumericTest)
	{
	public:
		
		TEST_METHOD(TestTrailingZerosFactorial)
		{
      Assert::AreEqual(1, TrailingZerosFactorial(5));
      Assert::AreEqual(2, TrailingZerosFactorial(10));
		}

    TEST_METHOD(TestParity)
		{
      Assert::AreEqual(0, Parity(0xfa));
      Assert::AreEqual(1, Parity(0xfb));
		}
    TEST_METHOD(TestSetBits)
		{
      Assert::AreEqual(3, SetBits(7));
      Assert::AreEqual(3, SetBits(145));
		}
	};
}
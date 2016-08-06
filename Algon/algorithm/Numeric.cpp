#include "Numeric.h"
#include <stdint.h>
/// <summary>
/// Compute the number of trailings zeros of n! 
/// </summary>
int TrailingZerosFactorial(uint32_t n)
{
  int k = 0;
  while (n>0)
  {
    k += (n / 5);
    n /= 5;
  }
  return k;
}

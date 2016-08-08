#pragma once
#include <stdlib.h>
#include <stdint.h>
#include "../common.h"
// #trailing zeros for n!
int TrailingZerosFactorial(uint32_t n);

inline
bool IsPowerOf2(size_t v)
{
  return (v) && (v & (v - 1)) == 0;
}

inline
int SetBits(size_t x)
{
#if defined(__GNUC__)
  return __builtin_popcount(x);
#else

  int count = 0;
  for (; x; count++)
  {
    x &= x - 1;
  }
  return count;
#endif 
}

/* O(1) parity computation trick*/
inline
int Parity(uint32_t v)
{
  v ^= v >> 16;
  v ^= v >> 8;
  v ^= v >> 4;
  v &= 0xf;
  return (0x6996 >> v) & 1;
}

inline 
uint16_t ConvertEndian16(uint16_t x)
{
  return (x >> 8) | (x << 8);
}

inline 
uint32_t ConvertEndian32(uint32_t x)
{
  return ((x >> 24) & 0xff) |
    (x << 8 & 0xff0000) |
    (x >> 8 & 0xff00) |
    (x << 24 & 0xff000000);
}

// get a random in 0..N 
inline 
int randome(size_t N)
{
  return rand() / (RAND_MAX / N + 1);
}


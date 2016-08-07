#include "Util.h"
#include <math.h>
#include <stdio.h>
#include "../common.h"
namespace colinli{
  /* Return the number of digits of 'v' when converted to string in radix 10.
  * See ll2string() for more information. */
  uint32_t digits10(uint64_t v) {
    if (v < 10) return 1;
    if (v < 100) return 2;
    if (v < 1000) return 3;
    if (v < 1000000000000UL) {
      if (v < 100000000UL) {
        if (v < 1000000) {
          if (v < 10000) return 4;
          return 5 + (v >= 100000);
        }
        return 7 + (v >= 10000000UL);
      }
      if (v < 10000000000UL) {
        return 9 + (v >= 1000000000UL);
      }
      return 11 + (v >= 100000000000UL);
    }
    return 12 + digits10(v / 1000000000000UL);
  }

  /* Like digits10() but for signed values. */
  uint32_t sdigits10(int64_t v) {
    if (v < 0) {
      /* Abs value of LLONG_MIN requires special handling. */
      uint64_t uv = (v != LLONG_MIN) ?
        (uint64_t)-v : ((uint64_t)LLONG_MAX) + 1;
      return digits10(uv) + 1; /* +1 for the minus. */
    }
    else {
      return digits10(v);
    }
  }

  /* Convert a long long into a string. Returns the number of
  * characters needed to represent the number.
  * If the buffer is not big enough to store the string, 0 is returned.
  *
  * Based on the following article (that apparently does not provide a
  * novel approach but only publicizes an already used technique):
  *
  * https://www.facebook.com/notes/facebook-engineering/three-optimization-tips-for-c/10151361643253920
  *
  * Modified in order to handle signed integers since the original code was
  * designed for unsigned integers. */
  int ll2string(char *dst, size_t dstlen, long long svalue) {
    static const char digits[201] =
      "0001020304050607080910111213141516171819"
      "2021222324252627282930313233343536373839"
      "4041424344454647484950515253545556575859"
      "6061626364656667686970717273747576777879"
      "8081828384858687888990919293949596979899";
    int negative;
    unsigned long long value;

    /* The main loop works with 64bit unsigned integers for simplicity, so
    * we convert the number here and remember if it is negative. */
    if (svalue < 0) {
      if (svalue != LLONG_MIN) {
        value = -svalue;
      }
      else {
        value = ((unsigned long long) LLONG_MAX) + 1;
      }
      negative = 1;
    }
    else {
      value = svalue;
      negative = 0;
    }

    /* Check length. */
    uint32_t const length = digits10(value) + negative;
    if (length >= dstlen) return 0;

    /* Null term. */
    uint32_t next = length;
    dst[next] = '\0';
    next--;
    while (value >= 100) {
      int const i = (value % 100) * 2;
      value /= 100;
      dst[next] = digits[i + 1];
      dst[next - 1] = digits[i];
      next -= 2;
    }

    /* Handle last 1-2 digits. */
    if (value < 10) {
      dst[next] = '0' + (uint32_t)value;
    }
    else {
      int i = (uint32_t)value * 2;
      dst[next] = digits[i + 1];
      dst[next - 1] = digits[i];
    }

    /* Add sign. */
    if (negative) dst[0] = '-';
    return length;
  }

  int  d2string(char* buf, size_t len, double value) {
    if (isnan(value)) {
      len = snprintf(buf, len, "nan");
    }
    else if (isinf(value)) {
      if (value < 0)
        len = snprintf(buf, len, "-inf");
      else
        len = snprintf(buf, len, "inf");
    }
    else if (value == 0) {
      /* See: http://en.wikipedia.org/wiki/Signed_zero, "Comparisons". */
      if (1.0 / value < 0)
        len = snprintf(buf, len, "-0");
      else
        len = snprintf(buf, len, "0");
    }
    else {
        len = snprintf(buf, len, "%.17g", value);
    }

    return len;
  }

}//end of colinli ns
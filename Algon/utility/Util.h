#pragma once
#include <string>
#include <sstream>
#include <cstdint>
#include <functional>
#include <utility>
#include "../common.h"
namespace colinli {

  /* Convert a double to a string representation. Returns the number of bytes
  * required. The representation should always be parsable by strtod(3).
  * This function does not support human-friendly formatting like ld2string
  * does. It is intented mainly to be used inside t_zset.c when writing scores
  * into a ziplist representing a sorted set. */
  int d2string(char *buf, size_t len, double value);
  uint32_t digits10(uint64_t v);
  uint32_t sdigits10(int64_t v);
  int ll2string(char *dst, size_t dstlen, long long svalue);



  /// <summary>
  /// Everything conversion to string
  /// </summary>
  template <typename TSrc>
std::string ToStr(TSrc src) {
  std::stringstream ss;
  ss << src;
  return ss.str();
}

template<>
inline
std::string ToStr(double src){
  char buf[DEFAULT_BUFSIZE];
  d2string(buf, sizeof buf, src);
  return std::string(buf);
}

template<>
inline
std::string ToStr(long long src){
  char buf[DEFAULT_BUFSIZE];
  ll2string(buf, sizeof buf, src);
  return std::string(buf);
}

template<>
inline
std::string ToStr(int src){
  return std::to_string(src);
}

template<>
inline
 std::string ToStr(unsigned int src){
  return std::to_string(src);
}

template<>
inline
std::string ToStr(bool b) {
  return b ? "True" : "False";
}

template<>
inline
std::string ToStr(const char* s) {
  return std::string(s);
}

template<>
inline
std::string ToStr(const std::string& s) {
  return s;
}

template<typename TSrc, typename ... TRest>
std::string ToStr(TSrc src, TRest&&... rest) {
  std::string r = ToStr(src);
  r += ToStr(std::forward<TRest>(rest)...);
  return r;
}

// Noncopyable mixin class 
class NonCopyable
{
protected:
  NonCopyable(){}
  ~NonCopyable(){}
private:
  NonCopyable(const NonCopyable&);
  NonCopyable& operator=(const NonCopyable&);
};

 

/// <summary>
/// Singleton mixin, should be thread-safe
/// </summary>
template <typename Child>
class Singleton : private NonCopyable
{
public:  
  static Child& Instance( )
  {
    static Child child;
    return child;
  }
protected:
  Singleton() = default;
  virtual ~Singleton(){}
};

 

}// end of colinli ns 

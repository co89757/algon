#pragma once
#include <stdexcept>
#include "Util.h"
#include <cstdarg>

namespace colinli {
  class FileIOException : std::runtime_error
  {
  public:
    explicit FileIOException(const char* m) : runtime_error(m){}
    virtual ~FileIOException(){}
  };

  class InvalidDataException : public std::logic_error
  {
  public:
    explicit InvalidDataException(const char* m) :logic_error(m){}
    virtual ~InvalidDataException(){}
  };

  class ArgumentNullException : public std::invalid_argument
  {
  public:
    explicit ArgumentNullException(const char* m) :invalid_argument(m){}
    virtual ~ArgumentNullException(){}
  };

  class ApplicationException : public std::runtime_error
  {
  public:
    explicit ApplicationException(const char* m) :runtime_error(m){}
    virtual ~ApplicationException(){}
  };

  template<typename TExcept, typename ... TArgs>
  void Throw(TArgs&&... rest) {

    throw TExcept(ToStr(std::forward<TArgs>(rest)...).c_str());
  }

  template<typename TExcept, typename TVal, typename ... TRest>
  void ThrowOnFalse(TVal&& value, TRest&&... rest) {
    if (!value) {
      Throw<TExcept>(std::forward<TRest>(rest)...);
    }
  }

  template<typename E>
  void Throwf(const char* fmt, ...) {
    char buf[DEFAULT_BUFSIZE];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof buf, fmt, ap);
    va_end(ap);
    throw E(std::string(buf).c_str());
  }

}// end of colinli ns

#define CHECK_THROW(cond, E) \
  ::colinli::ThrowOnFalse<E>((cond), "Check Failed: " #cond )
#pragma once
#include <stdexcept>
#include "Util.h"
#include <cstdarg>

#define NEW_RTEXCEPTION(E) class E: public std::runtime_error\
    {\
  public:\
   explicit E(const char* m): runtime_error(m){}\
   explicit E(const std::string& m): runtime_error(m){}\
   virtual ~E(){}\
    }

namespace colinli {

  NEW_RTEXCEPTION(NullArgumentError);

  NEW_RTEXCEPTION(FileIOException);

  NEW_RTEXCEPTION(InvalidDataException);

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